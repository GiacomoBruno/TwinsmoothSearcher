#pragma once

#include "BS/BS_thread_pool_light.hpp"
#include "tlx/container/btree_set.hpp"

#include <array>
#include <gmpxx.h>
#include <iostream>
#include <vector>
#include <map>

namespace globals {

//#define CHECK_BITSIZE
#define GLOBALS globals::SearcherGlobals::Instance()

enum class OPTIMIZATION_LEVELS {
    NO_OPTIMIZATION = 0,
    CONSTANT_RANGE_OPTIMIZATION = 1,
    GLOBAL_K_OPTIMIZATION = 2,
    VARIABLE_RANGE_OPTIMIZATION = 3
};

class SearcherGlobals {
   public:
    static SearcherGlobals &Instance()
    {
        static SearcherGlobals instance;
        return instance;
    }

    static constexpr int ChunkSize{ 100 };
    int MaxBitSize{ 1024 };
    int MinBitSizeToSave{ 0 };
    int MaxBitSizeToSave{ 1024 };
    int OptimizationType{ 0 };
    int Smoothness{ 0 };
    int MaxFileSize{ 20 };
    int MaxNForPrimeCalcs{4};

    [[maybe_unused]] int RangeStart{ 0 };
    [[maybe_unused]] int RangeEnd{ 0 };
    int RangeCurrent{ 0 };
    [[maybe_unused]] int RangeStep{ 0 };

    [[maybe_unused]] double KStart{ 0.0 };
    [[maybe_unused]] double KEnd{ 0.0 };
    double KCurrent{ 0.0 };
    [[maybe_unused]] double KStep{ 0.0 };


    std::vector<int>& GetRelevantPrimes();
    BS::thread_pool_light ThreadPool{};
    std::string OutputFile{};

   private:
    SearcherGlobals() = default;
    std::vector<int> RelevantPrimes{};
};

struct mpz_pointer_comparator {
    int operator()(mpz_class *l, mpz_class *r) const
    {
        return mpz_cmp(l->get_mpz_t(), r->get_mpz_t()); // < 0;
    }
};

using PSET = tlx::btree_set<mpz_class *, mpz_pointer_comparator>;
using PVEC = std::vector<mpz_class *>;
using CHUNKARR = std::array<mpz_class *, SearcherGlobals::ChunkSize>;
using CHUNKVEC = std::vector<CHUNKARR>;

std::string PrintOptimizationLevel(OPTIMIZATION_LEVELS level);

using time_point = std::chrono::system_clock::time_point;

std::string format_time_point(
    const time_point &time,
    const std::string &format);

void iteration_header_string(size_t work_set_size);

void iteration_tail_string(
    size_t work_set_size,
    size_t last_set_size,
    double iteration_duration);

void LoadConfigFile();

std::map<int, int> Factors(mpz_class* number);

} // namespace globals