#pragma once

#include "searcher.hpp"

namespace searcher {

    struct range_optimization {
    };


    template<>
    void generate_twins<range_optimization>(std::vector<mpz_class *> &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1, result;
        int r = 0;
        for (auto &x: chunk) {
            //forward
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            while (y != S.end() && r < RANGE_SIZE) {
                m1 = *x * **y;
                m1 += **y;
                delta = **y - *x;
                d = m1 % delta;

                if (d == 0) {
                    m1 /= delta;
                    m1 -= 1;
                    if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= MAX_BIT_SIZE) {
                        auto res = new mpz_class{m1};
                        if (!S.exists(res)) {
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
            if (z != x_iter)
                while (r < RANGE_SIZE) {
                    m1 = **z * *x;
                    m1 += *x;
                    delta = *x - **z;
                    d = m1 % delta;

                    if (d == 0) {
                        m1 /= delta;
                        m1 -= 1;

                        if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= MAX_BIT_SIZE) {
                            auto res = new mpz_class{m1};
                            if (!S.exists(res)) {
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