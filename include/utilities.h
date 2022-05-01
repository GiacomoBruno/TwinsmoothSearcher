#pragma once

#include "logger.h"

constexpr char const* OUTPUT_FOLDER = "./results/outputs";
constexpr char const* STATUS_FN = "stats";
constexpr char const* TWINSMOOTHS_FN = "twinsmooths";
constexpr char const* PRIMES_FN = "primes";



namespace utilities {

    extern unsigned long long SMOOTHNESS;
    extern int NUM_THREADS;
    extern int TOP_NUMBERS_AMOUNT;
    extern logger* LG;
    extern logger* TWINSMOOTH_OUTPUT;
    extern logger* PRIMES_OUTPUT;

    bool get_bool(const char *s);

    int get_int(const char *s);

    unsigned long long get_ulong(const char *s);

    double get_double(const char *s);

    void init_machine();

    std::string get_out_folder();

}