//
// Created by Giacomo Bruno on 29/04/22.
//

#pragma once
#include <gmp.h>
#include "bigint.h"

class bigfloat {
public:
    mpf_t number{};

    explicit bigfloat(int num);
    explicit bigfloat(long long num);
    explicit bigfloat(float num);
    explicit bigfloat(double num);
    explicit bigfloat(const bigint& num);
    bigfloat(const bigfloat& num);
    bigfloat();
    ~bigfloat();




    bigfloat& operator*=(const bigfloat& other);
    bigfloat& operator/=(const bigfloat& other);


    bigfloat& operator=(bigfloat&& num) noexcept;
    bigfloat& operator=(const bigfloat& num);

    friend std::ostream& operator<<(std::ostream& stream, const bigfloat& num);
};


