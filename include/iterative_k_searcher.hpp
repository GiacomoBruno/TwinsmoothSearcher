#pragma once
#include "searcher.hpp"
#include "k_searcher.hpp"
#include <vector>
namespace searcher {

    PSET quality_twins{};
    bool is_run_with_quality_twins = false;


    template<>
    void generate_twins<globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION>(std::vector<mpz_class *> &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1, result;

        //PSET::iterator y_bound{};
        //PSET::iterator z_bound{};

        mpz_class max_y;
        mpz_class max_z;



        //calculate the range limit based on k
        if (!chunk.empty())
        {
            {
                mpf_class f{*chunk[0]};
                f *= GLOBALS.KCurrent;

                max_y = f;

                //mpz_class yb{f};
                //y_bound = S.lower_bound(&yb);
            }
            {
                mpf_class f{*chunk[0]};
                f *= (2 - GLOBALS.KCurrent);

                max_z = f;
                //mpz_class zb{f};
                //z_bound = S.upper_bound(&zb);
            }
        } else return;

        for (auto* x: chunk) {
            size_t total_calculations{0};
            size_t successful_calculations{0};
            //forward
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            while (y != S.end() && **y < max_y)
            {
                total_calculations++;
                m1 = *x * **y;
                m1 += **y;
                delta = **y - *x;
                d = m1 % delta;

                if (d == 0) {
                    successful_calculations++;
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
            if (z != x_iter) {
                while (**z < max_z) {
                    total_calculations++;
                    m1 = **z * *x;
                    m1 += *x;
                    delta = *x - **z;
                    d = m1 % delta;

                    if (d == 0) {
                        m1 /= delta;
                        m1 -= 1;
                        successful_calculations++;
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

            if(!is_run_with_quality_twins) {
                if ((successful_calculations / (total_calculations / 100.0)) > 10.0) {
                    quality_twins.insert(x);
                }
                is_run_with_quality_twins = false;
            }
        }
    }


    template<>
    void iteration<globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION>(PVEC &io, PSET &S)
    {
        static bool need_swap = true;
        static double k_tmp = GLOBALS.KCurrent;

        if(S.size() < 100000)
        {
            GLOBALS.KCurrent = 2.0;
        }
        else if(need_swap)
        {
            need_swap = false;
            GLOBALS.KCurrent = k_tmp;
        }

        std::vector<std::vector<mpz_class *>> chunks{};
        generate_chunks(io, chunks);

        std::vector<PSET> temp_results(chunks.size());

        GLOBALS.ThreadPool.push_loop(0, chunks.size(), [&](int a, int b) {
            for (int i = a; i < b; ++i) {
                generate_twins<globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION>(chunks[i], S, temp_results[i]);
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

    template<>
    void execute<globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION>(PSET &S)
    {
        std::cout << "\nEXECUTING: " << PrintOptimizationLevel(globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION) << std::endl;
        std::cout << "SMOOTHNESS: " << GLOBALS.Smoothness << std::endl;
        std::cout << "THREAD COUNT: " << GLOBALS.ThreadPool.get_thread_count() << std::endl;

        std::vector<mpz_class *> work_set;
        for (int i = 1; i <= GLOBALS.Smoothness; i++) {
            auto x = new mpz_class{i};
            S.insert(x);
            work_set.emplace_back(x);
        }
        benchmark b;

        GLOBALS.KCurrent = GLOBALS.KStart;
        double current_k = GLOBALS.KCurrent;
        while(GLOBALS.KCurrent < GLOBALS.KEnd)
        {
            std::cout << "STARTING WITH K = " << GLOBALS.KCurrent << " WORK SET = " << work_set.size() << std::endl;
            while (!work_set.empty()) {
                b.start_bench();

                iteration<globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION>(work_set, S);

                std::cout << "\t\tfound: " << work_set.size() << " in: ";
                b.conclude_bench();
                std::cout << std::endl;
            }
            std::cout << "\tFOUND: " << S.size() << std::endl;

            //populate work_set
            for(auto* n : quality_twins)
            {
                work_set.push_back(n);
            }
            //increase kcurrent
            current_k += GLOBALS.KStep;
            GLOBALS.KCurrent =  current_k;
            is_run_with_quality_twins = true;
        }


    }

}