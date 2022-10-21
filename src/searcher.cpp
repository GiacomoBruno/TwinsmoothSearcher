#include "searcher.hpp"

#include <chrono>
#include <fstream>
#include <filesystem>
#include <cmath>
namespace searcher {

void generate_chunks(const std::vector<mpz_class *> &input, CHUNKVEC &output)
{
    int counter = 0;
    int idx = 0;
    output.emplace_back();
    for (const auto &i : input) {
        if (counter == GLOBALS.ChunkSize) {
            counter = 0;
            output.emplace_back();
            idx++;
        }
        output[idx][counter] = i;
        counter++;
    }
}

int last_array_element(const CHUNKARR &arr)
{
    if (arr[globals::SearcherGlobals::ChunkSize - 1] == nullptr)
        for (int i = 0; i < globals::SearcherGlobals::ChunkSize; i++)
            if (arr[i] == nullptr)
                return i;

    return globals::SearcherGlobals::ChunkSize;
}

template <OPTIMIZATION_LEVELS level>
void generate_twins(CHUNKARR &chunk, PSET &S, PSET &output)
{
    mpz_class d, delta, m1;
    int max = last_array_element(chunk);

    for (int i = 0; i < max; i++) {
        bool found_same = false;
        auto &x = chunk[i];
        auto iter = S.begin();
        while (iter != S.end()) {
            if (x == *iter) {
                found_same = true;
                std::advance(iter, 1);
                continue;
            }

            if (found_same)
                FORWARD_CALCS(x, iter);
            else
                BACKWARD_CALCS(x, iter);

            if (d == 0) {
                ADD_TO_SET();
            }

            std::advance(iter, 1);
        }
    }
}

void check_result_collisions(
    std::vector<PSET> &results,
    std::vector<PSET> &output)
{
    if (results.size() < 2)
        return;

    size_t length = results.size() - (results.size() % 2);
    output = std::vector<PSET>{ length / 2 };

    GLOBALS

        .ThreadPool.push_loop(
            0,
            length,
            [&](int a, int) {
                for (auto *x : results[a]) {
                    output[a / 2].insert(x);
                }
                for (auto *x : results[a + 1]) {
                    auto ins = output[a / 2].insert(x);
                    if (!ins.second)
                        delete x;
                }

                results[a].

                    clear();

                results[a + 1].

                    clear();
            },
            length / 2);
    GLOBALS

        .ThreadPool.

        wait_for_tasks();

    if (length !=
        results.

        size()

    ) {
        for (auto *x : results[length]) {
            auto ins = output[length / 2 - 1].insert(x);
            if (!ins.second)
                delete x;
        }
        results[length].

            clear();
    }
    results.

        clear();
}

template <OPTIMIZATION_LEVELS level>
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

    // parallelized check for collisions in the new results
    while (temp_results.size() > 1) {
        std::vector<PSET> IN{ temp_results };
        std::vector<PSET> OUT;
        check_result_collisions(IN, OUT);
        temp_results = OUT;
    }

    for (auto *n : temp_results[0]) {
        S.insert(n);
        io.push_back(n);
    }
}

template <OPTIMIZATION_LEVELS level>
void execute(PSET &S)
{
    std::cout << "\nEXECUTING: " << PrintOptimizationLevel(level) << std::endl;
    std::cout << "SMOOTHNESS: " << GLOBALS.Smoothness << std::endl;
    std::cout << "THREAD COUNT: " << GLOBALS.ThreadPool.get_thread_count()
              << std::endl;

    std::vector<mpz_class *> work_set;
    for (int i = 1; i <= GLOBALS.Smoothness; i++) {
        auto x = new mpz_class{ i };
        S.insert(x);
        work_set.emplace_back(x);
    }
    benchmark b;

    while (!work_set.empty()) {
        size_t last_set_size = work_set.size();
        iteration_header_string(last_set_size);

        b.start_bench();
        iteration<level>(work_set, S);
        b.conclude_bench();

        if(!work_set.empty())
            iteration_tail_string(
                work_set.size(), last_set_size, b.seconds_passed());

        b.start_bench();
        calculate_large_primes(work_set);
        b.conclude_bench();

        std::cout << "prime calculation took: " << b.seconds_passed() << std::endl;

    }
    std::cout << "FOUND: " << S.size() << std::endl;
}

void calculate_large_primes(const std::vector<mpz_class*>& numbers)
{
    if(numbers.empty()) return;
    if(mpz_sizeinbase((*numbers.rbegin())->get_mpz_t(), 2) < GLOBALS.MinBitSizeToSave) return;

    std::vector<mpz_class*> work_set{};
    for(auto iter = std::next(numbers.rbegin()); iter != numbers.rend(); std::advance(iter,1))
    {
        if(mpz_sizeinbase((*iter)->get_mpz_t(),2) < GLOBALS.MinBitSizeToSave) break;
        work_set.push_back(*iter);
    }
    std::vector<std::string> strings(work_set.size());
    Factors(nullptr);
    GLOBALS.ThreadPool.push_loop(0, work_set.size(), [&](size_t a, size_t b) {

        for(size_t i = a; i < b; ++i)
        {
            for(int n = 2; n < GLOBALS.MaxNForPrimeCalcs; n++)
            {
                auto p_prime = new mpz_class{};

                mpz_pow_ui(p_prime->get_mpz_t(), work_set[i]->get_mpz_t(), n);
                *p_prime = (2 * *p_prime) - 1;
                if(mpz_probab_prime_p(p_prime->get_mpz_t(), 50))
                {
                    auto prime = p_prime;
                    auto smooth = work_set[i];
                    std::stringstream ss;

                    mpz_class num{*prime};
                    mpz_class pp1{*prime};
                    mpz_class pm1{*prime};
                    mpz_class T{1};

                    pp1 += 1;
                    pm1 -= 1;

                    auto factors_pp1 = Factors(&pp1);
                    auto factors_pm1 = Factors(&pm1);
                    if(factors_pp1[2] < 40 && factors_pm1[2] < 40) {
                        continue;
                    }

                    ss << "\nPRIME: " << prime->get_str() << "\n";
                    ss << "BITSIZE: " << mpz_sizeinbase(prime->get_mpz_t(), 2) << "bits\n";

                    ss << "GENERATOR: N =" << n << " r = " << smooth->get_str() << "\n";
                    ss <<"\t2 * " << smooth->get_str() << "^" << n << " - 1" << "\n\n";

                    ss << "p + 1 = " << pp1.get_str() << "\n";

                    ss << "f(p+1) = " << factors_pp1[2] << "\n";
                    ss << "sqrt(B)/f(p+1) = " << (std::sqrt(factors_pp1.rbegin()->second) / static_cast<double>(factors_pp1[2])) << "\n";
                    ss << "p + 1 factors: ";
                    for(auto& fac : factors_pp1)
                    {
                        ss << fac.first << "^" << fac.second << " * ";
                        if(fac.first != 2)
                        {
                            mpz_class tmp{fac.first};
                            mpz_pow_ui(tmp.get_mpz_t(), tmp.get_mpz_t(), fac.second);
                            T *= tmp;
                        }
                    }
                    T = pp1 - T;
                    ss << "T (" << T.get_str() << ")\n";

                    ss << "p - 1 = " << pm1.get_str() << "\n";
                    ss << "f(p-1) = " << factors_pm1[2] << "\n";
                    ss << "sqrt(B)/f(p-1) = " << (sqrt(factors_pm1.rbegin()->second) / static_cast<double>(factors_pm1[2])) << "\n";
                    ss << "p - 1 factors: ";
                    T = 1;
                    for(auto& fac : factors_pm1)
                    {
                        ss << fac.first << "^" << fac.second << " * ";
                        if(fac.first != 2)
                        {
                            mpz_class tmp{fac.first};
                            mpz_pow_ui(tmp.get_mpz_t(), tmp.get_mpz_t(), fac.second);
                            T *= tmp;
                        }
                    }
                    T = pm1 - T;
                    ss << "T (" <<  T.get_str() << ")\n";

                    strings[i] = ss.str();
                }

                delete p_prime;

            }
        }

    });
    GLOBALS.ThreadPool.wait_for_tasks();

    std::filesystem::create_directories("./result/");
    std::ofstream interesting_primes("./result/"+ GLOBALS.OutputFile+"_primes_" + std::to_string(GLOBALS.Smoothness) + ".txt", std::ios::app);

    for(auto& s : strings)
    {
        interesting_primes << s;
    }

    interesting_primes.close();
}


template void execute<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(PSET &S);

template void iteration<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(
    PVEC &io,
    PSET &S);

template void generate_twins<OPTIMIZATION_LEVELS::NO_OPTIMIZATION>(
    CHUNKARR &chunk,
    PSET &S,
    PSET &output);

template void execute<OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>(
    PSET &S);

template void iteration<OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>(
    PVEC &io,
    PSET &S);

template void execute<OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION>(PSET &S);

template void execute<OPTIMIZATION_LEVELS::VARIABLE_RANGE_OPTIMIZATION>(
    PSET &S);

} // namespace searcher