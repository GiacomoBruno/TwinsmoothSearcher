#pragma once
#include <gmpxx.h>
#include "tlx/container/btree_set.hpp"
#include "BS/BS_thread_pool_light.hpp"

namespace globals
{

#define GLOBALS globals::SearcherGlobals::Instance()

    enum class OPTIMIZATION_LEVELS
    {
        NO_OPTIMIZATION = 0,
        CONSTANT_RANGE_OPTIMIZATION = 1,
        GLOBAL_K_OPTIMIZATION = 2,
        ITERATIVE_K_OPTIMIZATION = 3,
        ITERATIVE_RANGE_OPTIMIZATION = 4,
        VARIABLE_RANGE_OPTIMIZATION = 5
    };

    class SearcherGlobals
    {
    public:
        static SearcherGlobals& Instance() {
            static SearcherGlobals instance;
            return instance;
        }

        int ChunkSize{100};
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

    std::string PrintOptimizationLevel(OPTIMIZATION_LEVELS level)
    {
        using O = OPTIMIZATION_LEVELS;
        switch(level)
        {
            case O::NO_OPTIMIZATION:
                return "No optimizations";
            case O::CONSTANT_RANGE_OPTIMIZATION:
                return std::string("Constant range: ") + std::to_string(GLOBALS.RangeCurrent);
            case O::GLOBAL_K_OPTIMIZATION:
                return std::string("Global K: ") + std::to_string(GLOBALS.KCurrent);
            case O::ITERATIVE_K_OPTIMIZATION:
                return std::string("Iterative K: ") + std::to_string(GLOBALS.KStart) + " - " + std::to_string(GLOBALS.KEnd);
            case O::ITERATIVE_RANGE_OPTIMIZATION:
                return std::string("Iterative Range: ") + std::to_string(GLOBALS.RangeStart) + " - " + std::to_string(GLOBALS.RangeEnd);
            case O::VARIABLE_RANGE_OPTIMIZATION:
                return "Variable range";
        }
        return "";
    }
}