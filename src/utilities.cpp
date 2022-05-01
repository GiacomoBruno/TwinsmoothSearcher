//
// Created by Giacomo Bruno on 30/04/22.
//

#include "../include/utilities.h"
#include <iostream>
#include <thread>
#include <omp.h>

namespace utilities {

    unsigned long long SMOOTHNESS;
    int NUM_THREADS;
    int TOP_NUMBERS_AMOUNT;
    logger* LG;
    logger* TWINSMOOTH_OUTPUT;
    logger* PRIMES_OUTPUT;

    bool get_bool(const char *s) {
        std::cout << s;
        std::flush(std::cout);
        bool res;
        std::cin >> res;
        return res;
    }

    int get_int(const char *s) {
        std::cout << s;
        std::flush(std::cout);
        int res;
        std::cin >> res;
        return res;
    }

    unsigned long long get_ulong(const char *s) {
        std::cout << s;
        std::flush(std::cout);
        size_t res;
        std::cin >> res;
        return res;
    }

    double get_double(const char *s) {
        std::cout << s;
        std::flush(std::cout);
        double res;
        std::cin >> res;
        return res;
    }

    std::string get_out_folder() {
        return (std::string(OUTPUT_FOLDER) + "_" + std::to_string(SMOOTHNESS));
    }

    void init_machine()
    {
        NUM_THREADS = (int)std::thread::hardware_concurrency();
        SMOOTHNESS = get_ulong("choose smoothness: ");
        TOP_NUMBERS_AMOUNT = get_int("how many top numbers to log: ");
        LG = new logger(STATUS_FN, get_out_folder(), SMOOTHNESS);
        TWINSMOOTH_OUTPUT = new logger(TWINSMOOTHS_FN, get_out_folder(), SMOOTHNESS);
        PRIMES_OUTPUT = new logger(PRIMES_FN, get_out_folder(), SMOOTHNESS);

        omp_set_dynamic(0);
        omp_set_num_threads(NUM_THREADS);

        LG->logl("detected available threads: ", NUM_THREADS);
        LG->logl("smoothness: ", SMOOTHNESS);
        std::filesystem::create_directories(get_out_folder());
    }

}