#pragma once

#include "searcher.hpp"

namespace searcher {

    struct k_optimization {
    };


    template<>
    void generate_twins<k_optimization>(std::vector<mpz_class *> &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1, result;

        PSET::iterator y_bound{};
        PSET::iterator z_bound{};

        if (!chunk.empty()) {
            {
                mpf_class f{*chunk[0]};
                f *= k;
                mpz_class yb{f};
                y_bound = S.lower_bound(&yb);
            }
            {
                mpf_class f{*chunk[0]};
                f *= (2 - k);
                mpz_class zb{f};
                z_bound = S.upper_bound(&zb);
            }
        } else return;


        for (auto &x: chunk) {

            //forward
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            while (y != S.end() && y != y_bound) {
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
                std::advance(y, 1);
            }
            //backward
            if (z != x_iter)
                while (z != z_bound) {
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
                    if (z == S.begin()) break;
                    std::advance(z, -1);
                }
        }
    }


}