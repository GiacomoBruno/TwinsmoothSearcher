#pragma once
#include <gmp.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <omp.h>

#define NUM_THREADS 8
#define MPZ_INIT_BITS 128                                           /* Max bit-length for integers at start*/

#define MEGABYTE 1000000
#define MAX_FILE_SIZE 100*MEGABYTE

#define OUTPUT_FOLDER "./results/outputs"
#define WORK_FOLDER "./partial/partial"
#define TWINSMOOTH_FN "neighbors"
#define PRIMES_FN "primes"
#define LOOK_OLD_FILES true

#define OUT_FOLDER (std::string(OUTPUT_FOLDER)+"_"+std::to_string(smoothness))


#define bigint_new (bigint)malloc(sizeof(mpz_t))
#define bigint_free(n) mpz_clear(*n); free(n)
#define bigint_init(n,m) mpz_init2(*n, MPZ_INIT_BITS); mpz_set_ui(*n, m)
#define bigint_str(n,s) mpz_init2(*n, MPZ_INIT_BITS); mpz_set_str(*n, s.c_str(), 10)
typedef mpz_t* bigint;


#define CLEAR(a) a->clear(); delete a;
#define NULL_INIT_ARRAY(array, size) \
for(int i = 0; i < (size); i++) {      \
    (array)[i] = nullptr;              \
}

#define DEALLOCATE_ARRAY_MEMBERS(array, size) \
for(int i = 0; i < (size); i++) {               \
    if((array)[i] != nullptr){                  \
        delete (array)[i];                      \
        (array)[i] = nullptr;                   \
    }                                         \
}

#define INSERT_ARRAY_MEMBERS_INTO_TREE(array, tree, size) \
for(int i = 0; i < (size); i++) {                           \
    if((array)[i] != nullptr){                              \
        (tree)->simple_merge((array)[i]);                     \
    }                                                     \
}

#define REMOVE_FIRST_N_FROM_ARRAY(array, n) \
if((array)->size() <= (n) )\
    (array)->erase((array)->begin(), (array)->end());\
else \
    (array)->erase((array)->begin(), (array)->begin()+(n));

