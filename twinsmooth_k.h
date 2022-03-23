//
// Created by Giacomo Bruno on 16/03/22.
//

#pragma once
#include "twinsmooth.h"

class s_twinsmooth_k : public s_twinsmooth {
protected:
    double k_value;
    bigfloat k;
public:
    explicit s_twinsmooth_k(size_t s, double v);

    void start() override;
    void execute() override;
};