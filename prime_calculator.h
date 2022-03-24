//
// Created by Giacomo Bruno on 24/03/22.
//

#pragma once
#include "twinsmooth.h"

class prime_calculator : public s_twinsmooth {
protected:
    void load_files() override;
public:
    explicit prime_calculator(size_t s) : s_twinsmooth(s) {}

    void start() override;
    void execute() override;
    void terminate() override;



};


