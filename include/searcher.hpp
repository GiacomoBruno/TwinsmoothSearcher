#pragma once

#include "benchmark.hpp"
#include "global_definitions.hpp"

#include <iostream>
#include <vector>

#define FORWARD_CALCS(x, y) \
    do { \
        m1 = *(x) + 1; \
        m1 *= **(y); \
        delta = **(y) - *(x); \
        d = m1 % delta; \
    } while (false)

#define BACKWARD_CALCS(x, z) \
    do { \
        m1 = **(z) + 1; \
        m1 *= *(x); \
        delta = *(x) - **(z); \
        d = m1 % delta; \
    } while (false)

#ifdef CHECK_BITSIZE
#    define ADD_TO_SET() \
        do { \
            m1 /= delta; \
            m1 -= 1; \
            if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= GLOBALS.MaxBitSize) { \
                auto res = new mpz_class{ m1 }; \
                if (S.find(res) == S.end()) \
                    output.insert(res); \
                else \
                    delete res; \
            } \
        } while (false)
#else
#    define ADD_TO_SET() \
        do { \
            m1 /= delta; \
            m1 -= 1; \
            auto res = new mpz_class{ m1 }; \
            if (S.find(res) == S.end()) \
                output.insert(res); \
            else \
                delete res; \
        } while (false)
#endif

namespace searcher {

using namespace globals;

void generate_chunks(const std::vector<mpz_class *> &input, CHUNKVEC &output);

int last_array_element(const CHUNKARR &arr);

template <OPTIMIZATION_LEVELS level>
void generate_twins(CHUNKARR &chunk, PSET &S, PSET &output);

void check_result_collisions(
    std::vector<PSET> &results,
    std::vector<PSET> &output);

template <OPTIMIZATION_LEVELS level>
void iteration(PVEC &io, PSET &S);

template <OPTIMIZATION_LEVELS level>
void execute(PSET &S);

void calculate_large_primes(const std::vector<mpz_class*>& numbers);

} // namespace searcher
