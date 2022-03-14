#pragma once
#include <filesystem>
#include "twinsmooth.h"

class twinsmooth_kopt : public twinsmooth{
protected:
    bigfloat k1;
    bigfloat k2;
public:
    explicit twinsmooth_kopt(uint64_t s, double _k1, double _k2) : twinsmooth(s) {
        k1 = bigfloat_new;
        k2 = bigfloat_new;

        bigfloat_init(k1, _k1);
        bigfloat_init(k2, _k2);
    }
    void execute() override;
    void terminate() override;
    ~twinsmooth_kopt() override { bigfloat_free(k1); bigfloat_free(k2);};
};

