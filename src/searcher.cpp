#include "searcher.hpp"

namespace searcher {
    void generate_chunks(const std::vector<mpz_class*> &input, CHUNKVEC &output) {
        int counter = 0;
        int idx = 0;
        output.emplace_back();
        for (const auto &i: input) {
            if (counter == GLOBALS.ChunkSize) {
                counter = 0;
                output.emplace_back();
                idx++;
            }
            output[idx][counter] = i;
            counter++;
        }
    }

    int last_array_element(const CHUNKARR& arr)
    {
        if(arr[globals::SearcherGlobals::ChunkSize-1] == nullptr)
            for(int i = 0; i < globals::SearcherGlobals::ChunkSize; i++)
                if(arr[i] == nullptr)
                    return i;

        return globals::SearcherGlobals::ChunkSize;
    }

    template<OPTIMIZATION_LEVELS level>
    void generate_twins(CHUNKARR &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1;
        int max = last_array_element(chunk);

        for(int i = 0; i < max; i++)
        {
            bool found_same = false;
            auto&x = chunk[i];
            auto iter = S.begin();
            while(iter != S.end())
            {
                if(x == *iter)
                {
                    found_same = true;
                    std::advance(iter, 1);
                    continue;
                }

                if(found_same)
                    FORWARD_CALCS(x,iter);
                else
                    BACKWARD_CALCS(x,iter);

                if(d == 0)
                {
                    ADD_TO_SET();
                }

                std::advance(iter, 1);
            }
        }

    }



    void check_result_collisions(std::vector<PSET> &results, std::vector<PSET> &output) {
        if (results.size()< 2)
            return;

        size_t length = results.size() - (results.size() % 2);
        output = std::vector<PSET>{length / 2};

        GLOBALS

                .ThreadPool.push_loop(0, length, [&](
                int a,
                int) {
            for (
                auto *x
                    : results[a]) {
                output[a / 2].
                        insert(x);
            }
            for (
                auto *x
                    : results[a + 1]) {
                auto ins = output[a / 2].insert(x);
                if (!ins.second)
                    delete
                            x;
            }

            results[a].

                    clear();

            results[a + 1].

                    clear();

        }, length / 2);
        GLOBALS

                .ThreadPool.

                wait_for_tasks();

        if (length != results.

                size()

                ) {
            for (
                auto *x
                    : results[length]) {
                auto ins = output[length / 2 - 1].insert(x);
                if (!ins.second)
                    delete
                            x;
            }
            results[length].

                    clear();

        }
        results.

                clear();

    }

    template<OPTIMIZATION_LEVELS level>
    void iteration(PVEC &io, PSET &S)
    {
        CHUNKVEC chunks{};
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

    template void execute<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(PSET& S);
    template void iteration<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(PVEC &io, PSET &S);
    template void generate_twins<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(CHUNKARR &chunk, PSET &S, PSET &output);
    template void execute<OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>(PSET& S);
    template void iteration<OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>(PVEC &io, PSET &S);
    template void execute<OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION>(PSET& S);
    template void execute<OPTIMIZATION_LEVELS::VARIABLE_RANGE_OPTIMIZATION>(PSET& S);

}