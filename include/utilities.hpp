#pragma once

#include "logger.hpp"
#include <gmpxx.h>
constexpr char const* OUTPUT_FOLDER = "./results/outputs";
constexpr char const* STATUS_FN = "stats";
constexpr char const* TWINSMOOTHS_FN = "twinsmooths";
constexpr char const* PRIMES_FN = "primes";



namespace utilities {

    extern int SMOOTHNESS;
    extern int NUM_THREADS;
    extern int TOP_NUMBERS_AMOUNT;
    extern bool SAVE_ALL_TWINSMOOTHS;

    bool get_bool(const char *s);

    int get_int(const char *s);

    unsigned long long get_ulong(const char *s);

    double get_double(const char *s);

    void init_machine();

    std::string get_out_folder();

    bool is_prime_1(const mpz_class &num, mpz_class &output);

    bool is_prime_2(const mpz_class &num, mpz_class &output);

    bool is_prime_3(const mpz_class &num, mpz_class &output);

    bool is_prime_4(const mpz_class &num, mpz_class &output);

}