#pragma once
#include "twinsmooth.h"

class s_twinsmooth_k_growing : public s_twinsmooth {
protected:
    double start_k;
    double end_k;
    double step_k;
    double cur_k;
    bigfloat current_k;
    bigfloat old_k;


    void increment_k();
    void load_files() override;
public:
    s_twinsmooth_k_growing(size_t s, double sk, double ek, double ks);
    void start() override;
    void execute() override;
};