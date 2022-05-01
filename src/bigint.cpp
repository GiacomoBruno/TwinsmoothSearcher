//
// Created by Giacomo Bruno on 21/04/22.
//

#include "../include/bigint.h"

bigint::bigint() {
    mpz_init2(number, MPZ_INIT_BITS);
}

bigint::bigint(const bigint  &num) {
    mpz_init_set(number, num.number);
}

bigint::bigint(int num) {
    mpz_init_set_si(number, num);
}

bigint::bigint(long long num) {
    mpz_init_set_ui(number, num);
}

bigint::bigint(mpf_t *num) {
    mpz_init2(number, MPZ_INIT_BITS);
    mpz_set_f(number, *num);
}
bigint::~bigint() {
    mpz_clear(number);
}

bigint operator+(const bigint &number, const bigint &other) {
    bigint res{};
    mpz_add(res.number, number.number, other.number);
    return res;
}

bigint operator-(const bigint &number, const bigint &other) {
    bigint res{};
    mpz_sub(res.number, number.number, other.number);
    return res;
}

bigint operator/(const bigint& number, const bigint &other) {
    bigint res{};
    mpz_div(res.number, number.number, other.number);
    return res;
}

bigint operator*(const bigint& number, const bigint &other) {
    bigint res{};
    mpz_mul(res.number, number.number, other.number);
    return res;
}


int bigint::operator<=>(const bigint &other) const {
    return mpz_cmp(number, other.number);
}

int bigint::operator<=>(long long int other) const {
    return mpz_cmp_ui(number, other);
}

int bigint::operator<=>(int other) const {
    return mpz_cmp_si(number, other);
}

int bigint::operator<=>(const mpz_t& other) const {
    return mpz_cmp(number, other);
}

std::ostream& operator<<(std::ostream& stream, const bigint &num) {
    auto a = mpz_get_str(nullptr, 10, num.number);
    stream << a;
    free(a);
    return stream;

}

bigint& bigint::operator=(bigint &&other)  noexcept {
    if(this == &other)
        return *this;

    mpz_set(number, other.number);
    return *this;
}

bigint &bigint::operator+=(const bigint &other) {
    mpz_add(number, number, other.number);
    return *this;
}

bigint &bigint::operator*=(const bigint &other) {
    mpz_mul(number, number, other.number);
    return *this;
}

bigint &bigint::operator/=(const bigint &other) {
    mpz_div(number, number, other.number);
    return *this;
}

bigint &bigint::operator=(const bigint& other) {
    mpz_set(number, other.number);
    return *this;
}

bigint operator%(const bigint &num, const bigint &other) {
    bigint res{};
    mpz_mod(res.number, num.number, other.number);
    return res;
}



bigint &bigint::operator-=(int other) {
    mpz_sub_ui(number, number, other);
    return *this;
}

bigint operator/(const bigint &num, int other) {
    bigint res{};
    mpz_div_ui(res.number, num.number, other);
    return res;
}

bigint operator+(const bigint &num, int other) {
    bigint res{};
    mpz_add_ui(res.number, num.number, other);
    return res;
}

bigint &bigint::operator+=(int other) {
    mpz_add_ui(number, number, other);
    return *this;
}

bigint operator*(const bigint &num, int other) {
    bigint res{};
    mpz_mul_ui(res.number, num.number, other);
    return res;
}









