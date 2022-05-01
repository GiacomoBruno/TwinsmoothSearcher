#pragma once
#include <iostream>
#include <gmp.h>

constexpr int MPZ_INIT_BITS = 128;

class bigint {
public:
    mpz_t number{};

    explicit bigint(int num);
    explicit bigint(long long num);
    explicit bigint(mpf_t* num);
    bigint(const bigint& num);

    bigint();
    ~bigint();

    friend bigint operator+(const bigint& num, const bigint& other);
    friend bigint operator-(const bigint& num, const bigint& other);
    friend bigint operator/(const bigint& num, const bigint& other);
    friend bigint operator*(const bigint& num, const bigint& other);
    friend bigint operator%(const bigint& num, const bigint& other);

    friend bigint operator/(const bigint& num, int other);
    friend bigint operator+(const bigint& num, int other);
    friend bigint operator*(const bigint& num, int other);


    bigint& operator+=(const bigint& other);
    bigint& operator*=(const bigint& other);
    bigint& operator/=(const bigint& other);

    bigint& operator-=(int other);
    bigint& operator+=(int other);

    int operator<(const bigint& other) const;
    int operator<(long long other) const;
    int operator<(int other) const;
    int operator<(const mpz_t& other) const;


    bigint& operator=(bigint&& num) noexcept;
    bigint& operator=(const bigint& num);

    friend std::ostream& operator<<(std::ostream& stream, const bigint& num);
};
