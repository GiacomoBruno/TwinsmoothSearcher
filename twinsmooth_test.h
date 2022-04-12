#pragma once
#include "twinsmooth.h"

class twinsmooth_test : public s_twinsmooth {


public:
    explicit twinsmooth_test(size_t s) : s_twinsmooth(s) {}
    void execute() override;

};
