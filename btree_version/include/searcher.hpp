#pragma once

#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <string_view>
#include "tlx/container/btree_set.hpp"
#include "BS/BS_thread_pool_light.hpp"
#include <set>
#include <map>
#include "benchmark.hpp"

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

    struct no_optimization{};

    namespace {
        struct mpz_pointer_comparator {
            auto operator()(mpz_class *l, mpz_class *r) const {
                return mpz_cmp(l->get_mpz_t(), r->get_mpz_t());
            }

            auto operator()(const mpz_class *l, const mpz_class *r) const {
                return mpz_cmp(l->get_mpz_t(), r->get_mpz_t());
            }
        };

        struct mpz_pointer_comparator_for_set {
            auto operator()(mpz_class *l, mpz_class *r) const {
                return mpz_cmp(l->get_mpz_t(), r->get_mpz_t()) < 0;
            }

            auto operator()(const mpz_class *l, const mpz_class *r) const {
                return mpz_cmp(l->get_mpz_t(), r->get_mpz_t()) < 0;
            }
        };
    }

    using PSET = tlx::btree_set<mpz_class*, mpz_pointer_comparator>;
    //using PSET = std::set<mpz_class*, mpz_pointer_comparator_for_set>;
    using PVEC = std::vector<mpz_class *>;

    int CHUNK_SIZE = 100;
    int RANGE_SIZE = 2500;
    int MAX_BIT_SIZE = 1024;
    int MIN_BIT_SIZE_TO_SAVE = 0;
    int MAX_BIT_SIZE_TO_SAVE = 1024;
    int OPTIMIZATION = 0;
    int SMOOTHNESS = 0;
    BS::thread_pool_light thread_pool;
    std::string output_file;
    double k = 2.0;

    template<typename T>
    void generate_chunks(const std::vector<T> &input, std::vector<std::vector<T>> &output) {
        int counter = 0;
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
                        if (S.find(res) == S.end()) {
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
                            if (S.find(res) == S.end()) {
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
    void iteration(PVEC &io, PSET &S)
    {
        std::vector<std::vector<mpz_class *>> chunks{};
        generate_chunks(io, chunks);

        std::vector<PSET> temp_results(chunks.size());



        auto loop = [&](int a, int b) {
            for (int i = a; i < b; ++i) {
                generate_twins<T>(chunks[i], S, temp_results[i]);
            }
        };

        thread_pool.push_loop(0, chunks.size(), loop);

        thread_pool.wait_for_tasks();

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

    void get_some_stats(PSET& S)
    {
        std::map<int, int> bitsizes{};

        for(auto n : S)
        {
            int size = mpz_sizeinbase(n->get_mpz_t(), 2);

            if(bitsizes.find(size) == bitsizes.end())
                bitsizes[size] = 1;
            else
                bitsizes[size] += 1;
        }

        std::cout << "BITS->     AMOUNT\n";


        for(auto bsize : bitsizes)
        {
            std::cout << std::setw(3) << bsize.first << " -> " << std::setw(10) << bsize.second << std::endl;
        }
    }

    std::map<size_t,size_t> generate_bitsize_range_map(PSET& S)
    {
        std::map<size_t, std::vector<size_t>> allMap;

        for(auto x : S)
        {
            mpz_class low = *x / 2;
            mpz_class high = *x * 2;

            auto low_iter = S.upper_bound(&low);
            auto high_iter = S.lower_bound((&high));


            size_t counter = 0;
            auto x_iter = S.find(x);
            auto iter = x_iter;
            while(iter != S.end() && iter != low_iter)
            {
                counter++;
                std::advance(iter, -1);
            }
            iter = x_iter;
            while(iter != S.end() && iter != high_iter)
            {
                counter++;
                std::advance(iter, 1);
            }

            auto bitsize = mpz_sizeinbase(x->get_mpz_t(), 2);

            if(allMap.find(bitsize) == allMap.end())
                allMap[bitsize] = {};

            allMap[bitsize].push_back(counter);
        }
        std::map<size_t, size_t> result{};

        for(const auto& bitsize : allMap)
        {
            size_t average = 0;
            for(auto count : bitsize.second)
                average += count;

            average = average / bitsize.second.size();
            result[bitsize.first] = average;
        }

        for(size_t i = 0; i < 80; i++)
        {
            if(result.find(i) == result.end())
                result[i] = 0;
        }

        return result;

    }
    template<typename T>
    void execute(int s, PSET &S)
    {
        std::cout << "\nEXECUTING: " << type_name<T>() <<std::endl;
        std::cout << "SMOOTHNESS: " << s << std::endl;
        std::cout << "THREAD COUNT: "<< thread_pool.get_thread_count() << std::endl;

        std::vector<mpz_class *> work_set;
        for (int i = 1; i <= s; i++) {
            auto x = new mpz_class{i};
            S.insert(x);
            work_set.emplace_back(x);
        }
        benchmark b;

        while (!work_set.empty()) {
            b.start_bench();
            iteration<T>(work_set, S);

            std::cout << "found: " << work_set.size() << " in: ";
            b.conclude_bench();
            std::cout << std::endl;
        }
        std::cout << "FOUND: " << S.size() << std::endl;

        //get_some_stats(S);
        //return generate_bitsize_range_map(S);
    }

}
