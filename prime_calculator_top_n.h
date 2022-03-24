//
// Created by Giacomo Bruno on 24/03/22.
//

#pragma once
#include "prime_calculator.h"
#include "static_twinsmooth.h"

class prime_calculator_top_n : public s_twinsmooth {
protected:
    size_t primes_to_find;
    bool already_ordered = false;
public:
    explicit prime_calculator_top_n(size_t s,size_t n, bool ordered) : s_twinsmooth(s), primes_to_find(n), already_ordered(ordered) {}

    void start() override;
    void execute() override;
    void terminate() override;
};
