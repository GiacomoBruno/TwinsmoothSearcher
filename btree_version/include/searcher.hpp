#pragma once

#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <string_view>
#include "tlx/container/btree_set.hpp"
#include "BS/BS_thread_pool_light.hpp"

template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

namespace searcher {

    using SET = tlx::btree_set<mpz_class>;

    struct mpz_pointer_comparator {
        auto operator()(mpz_class *l, mpz_class *r) const {
            return mpz_cmp(l->get_mpz_t(), r->get_mpz_t());
        }

        auto operator()(const mpz_class *l, const mpz_class *r) const {
            return mpz_cmp(l->get_mpz_t(), r->get_mpz_t());
        }
    };

    using PSET = tlx::btree_set<mpz_class *, mpz_pointer_comparator>;

    using PVEC = std::vector<mpz_class *>;

    int CHUNK_SIZE = 100;
    int RANGE_SIZE = 2500;
    int MAX_BIT_SIZE = 1024;
    int MAX_BIT_SIZE_TO_SAVE = 1024;
    int OPTIMIZATION = 0;
    double k = 1.1;

    template<typename T>
    void generate_chunks(const std::vector<T> &input, std::vector<std::vector<T>> &output) {
        int counter;
        int idx = 0;
        output.emplace_back();

        for (const auto &i: input) {
            if (counter == CHUNK_SIZE) {
                counter = 0;
                output.emplace_back();
                idx++;
            }

            output[idx].push_back(i);
            counter++;
        }
    }

    template<typename T>
    void generate_twins(PVEC &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1;

        for (auto &x: chunk) {
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            while (y != S.end()) {
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
                while (true) {
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


    template<typename T>
    void iteration(PVEC &io, PSET &S) {
        std::vector<std::vector<mpz_class *>> chunks{};
        generate_chunks(io, chunks);

        PSET temp_results[chunks.size()];
        BS::thread_pool_light pool(120);

        auto loop = [&](int a, int b) {
            for (int i = a; i < b; ++i) {
                generate_twins<T>(chunks[i], S, temp_results[i]);
            }
        };

        pool.push_loop(0, chunks.size(), loop);

        pool.wait_for_tasks();

        io.clear();

        for (int i = 0; i < chunks.size(); i++) {
            for (auto &n: temp_results[i]) {
                auto ins = S.insert(n);
                if (ins.second) {
                    io.push_back(*ins.first);
                } else {
                    delete n;
                }
            }
        }
    }


    template<typename T>
    void execute(int s, PSET &S) {

        std::cout << "EXECUTING: " << type_name<T>() <<std::endl;

        std::vector<mpz_class *> work_set;
        for (int i = 1; i <= s; i++) {
            auto x = new mpz_class{i};
            S.insert(x);
            work_set.emplace_back(x);
        }

        while (!work_set.empty()) {
            iteration<T>(work_set, S);
            std::cout << "found: " << work_set.size() << std::endl;
        }

        std::cout << "FOUND: " << S.size() << std::endl;
    }

}
