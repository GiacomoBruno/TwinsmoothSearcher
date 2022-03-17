#pragma once
#include "twinsmooth.h"

class twinsmooth_k_growing : public twinsmooth {
private:
    double start_k, end_k, step_k, cur_k;

    bigfloat current_k, old_k;

    void set_k(double n);
public:
    explicit twinsmooth_k_growing(uint64_t s, double startk, double endk, double stepk);

    void start() override;
    void execute() override;
    void terminate() override;
};