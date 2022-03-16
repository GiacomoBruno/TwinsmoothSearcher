//
// Created by Giacomo Bruno on 16/03/22.
//

#pragma once
#include "twinsmooth_k.h"

class twinsmooth_k_growing : public twinsmooth {
protected:
    twinsmooth_k* executor;

    double kstart;
    double kend;
    double kstep;


public:
    twinsmooth_k_growing(uint64_t s, double _k1, double _k2, double _kstep) : twinsmooth(s), kstart(_k1), kend(_k2), kstep(_kstep)
    { executor = new twinsmooth_k(s); }

    void execute() override;
    void terminate() override {};

};
