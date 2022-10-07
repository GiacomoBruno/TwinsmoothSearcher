#pragma once
#include "global_definitions.hpp"
#include <iostream>
#include <vector>
#include "benchmark.hpp"

namespace searcher {

    using namespace globals;

    template<typename T>
    void generate_chunks(const std::vector<T> &input, std::vector<std::vector<T>> &output) {
        int counter = 0;
        int idx = 0;
        output.emplace_back();
        for (const auto &i: input) {
            if (counter == GLOBALS.ChunkSize) {
                counter = 0;
                output.emplace_back();
                idx++;
            }
            output[idx].push_back(i);
            counter++;
        }
    }

    template<OPTIMIZATION_LEVELS level>
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

                    if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= GLOBALS.MaxBitSize) {
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

                        if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= GLOBALS.MaxBitSize) {
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

    void check_result_collisions(std::vector<PSET>& results, std::vector<PSET>& output)
    {
        if(results.size() < 2)
            return;

        size_t length = results.size() - (results.size() % 2);
        output = std::vector<PSET>{length/2};

        GLOBALS.ThreadPool.push_loop(0, length, [&](int a, int) {
            for(auto* x : results[a])
            {
                output[a/2].insert(x);
            }
            for(auto* x : results[a+1])
            {
                auto ins = output[a/2].insert(x);
                if(!ins.second)
                    delete x;
            }

            results[a].clear();
            results[a+1].clear();
        }, length/2);
        GLOBALS.ThreadPool.wait_for_tasks();

        if(length != results.size())
        {
            for(auto* x : results[length]) {
                auto ins = output[length/2 -1].insert(x);
                if(!ins.second)
                    delete x;
            }
            results[length].clear();
        }
        results.clear();
    }

    template<OPTIMIZATION_LEVELS level>
    void iteration(PVEC &io, PSET &S)
    {
        std::vector<std::vector<mpz_class *>> chunks{};
        generate_chunks(io, chunks);

        std::vector<PSET> temp_results(chunks.size());

        GLOBALS.ThreadPool.push_loop(0, chunks.size(), [&](int a, int b) {
            for (int i = a; i < b; ++i) {
                generate_twins<level>(chunks[i], S, temp_results[i]);
            }
        });
        GLOBALS.ThreadPool.wait_for_tasks();

        io.clear();

        //parallelized check for collisions in the new results
        while(temp_results.size() > 1) {
            std::vector<PSET> IN{temp_results};
            std::vector<PSET> OUT;
            check_result_collisions(IN, OUT);
            temp_results = OUT;
        }

        for (auto* n: temp_results[0]) {
            S.insert(n);
            io.push_back(n);
        }
    }

    template<OPTIMIZATION_LEVELS level>
    void execute(PSET &S)
    {
        std::cout << "\nEXECUTING: " << PrintOptimizationLevel(level) << std::endl;
        std::cout << "SMOOTHNESS: " << GLOBALS.Smoothness << std::endl;
        std::cout << "THREAD COUNT: " << GLOBALS.ThreadPool.get_thread_count() << std::endl;

        std::vector<mpz_class *> work_set;
        for (int i = 1; i <= GLOBALS.Smoothness; i++) {
            auto x = new mpz_class{i};
            S.insert(x);
            work_set.emplace_back(x);
        }
        benchmark b;

        while (!work_set.empty()) {
            b.start_bench();
            iteration<level>(work_set, S);

            std::cout << "found: " << work_set.size() << " in: ";
            b.conclude_bench();
            std::cout << std::endl;
        }
        std::cout << "FOUND: " << S.size() << std::endl;
    }

}
