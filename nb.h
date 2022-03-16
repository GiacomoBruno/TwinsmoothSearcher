#pragma once
#include <gmp.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <omp.h>
#include <chrono>

#define NUM_THREADS 8
#define MPZ_INIT_BITS 128                                           /* Max bit-length for integers at start*/
#define MEGABYTE 1000000
#define MAX_FILE_SIZE MEGABYTE*100

#define DONT_SAVE_FILE true

#define CHUNK_SIZE 100
#define OUTPUT_FOLDER "./results/outputs"
#define TWINSMOOTH_FN "neighbors"
#define PRIMES_FN "primes"

#define OUT_FOLDER(n) (std::string(OUTPUT_FOLDER)+"_"+std::to_string(n))


#define bigint_new (bigint)malloc(sizeof(mpz_t))
#define bigint_free(n) mpz_clear(*n); free(n)
#define bigint_init(n,m) mpz_init2(*n, MPZ_INIT_BITS); mpz_set_ui(*n, m)

#define bigfloat_new (bigfloat)malloc(sizeof(mpf_t))
#define bigfloat_free(n) mpf_clear(*n); free(n)
#define bigfloat_init(n,m) mpf_init_set_d(*n, m)

typedef mpf_t* bigfloat;
typedef mpz_t* bigint;

#define NULL_INIT_ARRAY(array, size)   \
for(int i = 0; i < (size); i++) {      \
    (array)[i] = nullptr;              \
}

#define VAL(n) ((Node)((n)->value))


#define ELAPSED(START,END) (std::chrono::duration_cast<std::chrono::microseconds>(END - START).count() / (double)1000000)
#define CURRENT_TIME std::chrono::steady_clock::now()
typedef std::chrono::steady_clock::time_point t;


void start_bench();

void conclude_bench();


struct node {
    node* left = nullptr;
    node* right = nullptr;

    node* next = nullptr;
    node* prev = nullptr;

    int8_t height = 1;

    bigint val = nullptr;

    explicit node(bigint v) : val(v){}

    node* rotate_right();
    node* rotate_left();
    size_t distance(node* other);
    node* skip(size_t n);
    node* skip_back(size_t n);
};

typedef node* Node;