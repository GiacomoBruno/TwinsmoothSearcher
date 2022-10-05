#pragma once

#include "searcher.hpp"

namespace searcher {

    template<>
    void generate_twins<globals::OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>
            (std::vector<mpz_class *> &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1, result;
        int r = 0;
        for (auto &x: chunk) {
            //forward
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            while (y != S.end() && r < GLOBALS.RangeCurrent) {
                m1 = *x * **y;
                m1 += **y;
                delta = **y - *x;
                d = m1 % delta;

                if (d == 0) {
                    m1 /= delta;
                    m1 -= 1;
                    if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= GLOBALS.MaxBitSize) {
                        auto res = new mpz_class{m1};
                        if (S.find(res) == S.end()) {
                            output.insert(res);
                        } else {
                            delete res;
                        }
                    }
                }
                r++;
                std::advance(y, 1);
            }
            //backward
            r = 0;
            if (z != x_iter) {
                while (r < GLOBALS.RangeCurrent) {
                    m1 = **z * *x;
                    m1 += *x;
                    delta = *x - **z;
                    d = m1 % delta;

                    if (d == 0) {
                        m1 /= delta;
                        m1 -= 1;

                        if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= GLOBALS.MaxBitSize) {
                            auto res = new mpz_class{m1};
                            if (S.find(res) == S.end()) {
                                output.insert(res);
                            } else {
                                delete res;
                            }
                        }
                    }
                    r++;
                    if (z == S.begin()) break;
                    std::advance(z, -1);
                }
            }
        }
    }

}