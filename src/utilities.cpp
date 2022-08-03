#include "utilities.hpp"
#include <iostream>
#include <thread>
#include <omp.h>
#include <filesystem>

namespace utilities
{

    int SMOOTHNESS;
    int NUM_THREADS;
    int TOP_NUMBERS_AMOUNT;
    bool SAVE_ALL_TWINSMOOTHS;

    bool get_bool(const char *s)
    {
        std::cout << s;
        std::flush(std::cout);
        bool res;
        std::cin >> res;
        return res;
    }

    int get_int(const char *s)
    {
        std::cout << s;
        std::flush(std::cout);
        int res;
        std::cin >> res;
        return res;
    }

    unsigned long long get_ulong(const char *s)
    {
        std::cout << s;
        std::flush(std::cout);
        size_t res;
        std::cin >> res;
        return res;
    }

    double get_double(const char *s)
    {
        std::cout << s;
        std::flush(std::cout);
        double res;
        std::cin >> res;
        return res;
    }

    std::string get_out_folder()
    {
        return (std::string(OUTPUT_FOLDER) + "_" + std::to_string(SMOOTHNESS));
    }

    bool is_prime_1(const mpz_class &num, mpz_class &output)
    {
        output = (num * 2) + 1;

        return mpz_probab_prime_p(output.get_mpz_t(), 1000);
    }

    bool is_prime_2(const mpz_class &num, mpz_class &output)
    {
        output = num * num * 2 - 1;

        if (mpz_probab_prime_p(output.get_mpz_t(), 1000))
            return true;

        output = (num + 1) * (num + 1) * 2 - 1;

        return mpz_probab_prime_p(output.get_mpz_t(), 1000);
    }

    bool is_prime_3(const mpz_class &num, mpz_class &output)
    {
        output = num * num * num * 2 - 1;
        if (mpz_probab_prime_p(output.get_mpz_t(), 1000))
            return true;

        output = (num + 1) * (num + 1) * (num + 1) * 2 - 1;

        return mpz_probab_prime_p(output.get_mpz_t(), 1000);
    }

    bool is_prime_4(const mpz_class &num, mpz_class &output)
    {
        output = num * num * num * num * 2 - 1;
        if (mpz_probab_prime_p(output.get_mpz_t(), 1000))
            return true;

        output = (num + 1) * (num + 1) * (num + 1) * (num + 1) * 2 - 1;

        return mpz_probab_prime_p(output.get_mpz_t(), 1000);
    }

    void init_machine()
    {
        NUM_THREADS = (int)std::thread::hardware_concurrency();
        SMOOTHNESS = get_int("choose smoothness: ");
        //SAVE_ALL_TWINSMOOTHS = get_bool("save all twinsmooth found?(1 = yes, 0 = no) :");
        TOP_NUMBERS_AMOUNT = get_int("how many bits should the saved number have: ");

        omp_set_dynamic(0);
        omp_set_num_threads(NUM_THREADS);
        twins::simple_logger log(SMOOTHNESS);

        log.print("detected available threads: ");
        log.print(NUM_THREADS);

        log.print("\nsmoothness: ");
        log.print(SMOOTHNESS);
        log.print("\n");
        std::filesystem::create_directories(get_out_folder());
    }

}