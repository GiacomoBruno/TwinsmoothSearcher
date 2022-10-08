#pragma once
#include <gmpxx.h>
#include "tlx/container/btree_set.hpp"
#include "BS/BS_thread_pool_light.hpp"
#include <array>
#include <vector>

namespace globals
{

//#define CHECK_BITSIZE
#define GLOBALS globals::SearcherGlobals::Instance()

    enum class OPTIMIZATION_LEVELS
    {
        NO_OPTIMIZATION = 0,
        CONSTANT_RANGE_OPTIMIZATION = 1,
        GLOBAL_K_OPTIMIZATION = 2,
        VARIABLE_RANGE_OPTIMIZATION = 3
    };

    class SearcherGlobals
    {
    public:
        static SearcherGlobals& Instance() {
            static SearcherGlobals instance;
            return instance;
        }

        static constexpr int ChunkSize{100};
        int MaxBitSize{1024};
        int MinBitSizeToSave{0};
        int MaxBitSizeToSave{1024};
        int OptimizationType{0};
        int Smoothness{0};

        int RangeStart{0};
        int RangeEnd{0};
        int RangeCurrent{0};
        int RangeStep{0};

        double KStart{0.0};
        double KEnd{0.0};
        double KCurrent{0.0};
        double KStep{0.0};

        BS::thread_pool_light ThreadPool{};
        std::string OutputFile{};
    private:
        SearcherGlobals() = default;
    };

    struct mpz_pointer_comparator {
        int operator()(mpz_class *l, mpz_class *r) const {
            return mpz_cmp(l->get_mpz_t(), r->get_mpz_t());// < 0;
        }
    };

    using PSET = tlx::btree_set<mpz_class*, mpz_pointer_comparator>;
    using PVEC = std::vector<mpz_class *>;
    using CHUNKARR = std::array<mpz_class*, SearcherGlobals::ChunkSize>;
    using CHUNKVEC = std::vector<CHUNKARR>;

    std::string PrintOptimizationLevel(OPTIMIZATION_LEVELS level);
}