#pragma once
#include <gmp.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <omp.h>

#define NUM_THREADS 4
#define MPZ_INIT_BITS 128                                           /* Max bit-length for integers at start*/
#define MEGABYTE 1000000
#define MAX_FILE_SIZE MEGABYTE*100

#define OUTPUT_FOLDER "./results/outputs"
#define TWINSMOOTH_FN "neighbors"
#define PRIMES_FN "primes"

#define OUT_FOLDER(n) (std::string(OUTPUT_FOLDER)+"_"+std::to_string(n))


#define bigint_new (bigint)malloc(sizeof(mpz_t))
#define bigint_free(n) mpz_clear(*n); free(n)
#define bigint_init(n,m) mpz_init2(*n, MPZ_INIT_BITS); mpz_set_ui(*n, m)
typedef mpz_t* bigint;

#define NULL_INIT_ARRAY(array, size)   \
for(int i = 0; i < (size); i++) {      \
    (array)[i] = nullptr;              \
}

#define VAL(n) ((Node)((n)->value))

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
};

typedef node* Node;