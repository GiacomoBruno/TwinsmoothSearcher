//
// Created by Giacomo Bruno on 27/03/22.
//

#ifndef BSMOOTH_TWINSMOOTH_TEST_H
#define BSMOOTH_TWINSMOOTH_TEST_H
#include "twinsmooth.h"

class twinsmooth_test : public s_twinsmooth {


public:
    explicit twinsmooth_test(size_t s) : s_twinsmooth(s) {}
    void execute() override;

};


#endif //BSMOOTH_TWINSMOOTH_TEST_H
