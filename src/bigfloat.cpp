//
// Created by Giacomo Bruno on 29/04/22.
//

#include "../include/bigfloat.h"

bigfloat::bigfloat() {
    mpf_init2(number, MPZ_INIT_BITS);
}

bigfloat::bigfloat(int num) {
    mpf_init_set_si(number, num);
}

bigfloat::bigfloat(long long num) {
    mpf_init_set_ui(number, num);
}

bigfloat::bigfloat(float num) {
    mpf_init_set_d(number, num);
}

bigfloat::bigfloat(double num) {
    mpf_init_set_d(number, num);
}

bigfloat::bigfloat(const bigfloat &num) {
    mpf_init_set(number, num.number);
}

bigfloat &bigfloat::operator*=(const bigfloat &other) {
    mpf_mul(number, number, other.number);
    return *this;
}

bigfloat &bigfloat::operator/=(const bigfloat &other) {
    mpf_div(number, number, other.number);
    return *this;
}

bigfloat &bigfloat::operator=(bigfloat &&other) noexcept {
    if(this == &other)
        return *this;

    mpf_set(number, other.number);
    return *this;
}

bigfloat &bigfloat::operator=(const bigfloat &other) {
    mpf_set(number, other.number);
    return *this;
}

bigfloat::~bigfloat() {
    mpf_clear(number);
}

bigfloat::bigfloat(const bigint &num) {
    mpf_init2(number, MPZ_INIT_BITS);
    mpf_set_z(number, num.number);
}

std::ostream &operator<<(std::ostream &stream, const bigfloat &num) {
    char buf[1024];
    gmp_sprintf(buf, "%*Ff", num.number);
    stream << buf;
    return stream;
}


