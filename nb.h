#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <omp.h>
#include "type_definitions.h"
#include "logger.h"


#define NUM_THREADS 8
#define MPZ_INIT_BITS 128                                           /* Max bit-length for integers at start*/
#define MEGABYTE 1000000
#define MAX_FILE_SIZE MEGABYTE*100

#define DONT_SAVE_FILE true

#define CHUNK_SIZE 100
#define OUTPUT_FOLDER "./results/outputs"
#define STATUS_FOLDER "./results/status"
#define TWINSMOOTH_FN "neighbors"
#define STATUS_FN "stats"
#define PRIMES_FN "primes"
#define LOG(s) log->log(s)
#define LOGNL log->newline()
#define OUT_FOLDER(n) (std::string(OUTPUT_FOLDER)+"_"+std::to_string(n))
#define STAT_FOLDER(n) (std::string(STATUS_FOLDER)+"_"+std::to_string(n))

#define bigint_new (bigint)malloc(sizeof(mpz_t))
#define bigint_free(n) mpz_clear(*n); free(n)
#define bigint_init(n,m) mpz_init2(*n, MPZ_INIT_BITS); mpz_set_ui(*n, m)

#define bigfloat_new (bigfloat)malloc(sizeof(mpf_t))
#define bigfloat_free(n) mpf_clear(*n); free(n)
#define bigfloat_init(n,m) mpf_init_set_d(*n, m)
#define bigfloat_set(n,m) mpf_set_d(*n, m)


#define NULL_INIT_ARRAY(array, size)   \
for(int i = 0; i < (size); i++) {      \
    (array)[i] = nullptr;              \
}

#define VAL(n) ((Node)((n)->value))

#define ELAPSED(START,END) (std::chrono::duration_cast<std::chrono::microseconds>((END) - (START)).count() / (double)1000000)
#define CURRENT_TIME std::chrono::steady_clock::now()

class benchmark {
private:
    t start_time, end_time;
    clock_t st, et;

public:
    void start_bench();
    void conclude_bench();

};


