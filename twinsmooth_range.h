#pragma once
#include "twinsmooth.h"

class s_twinsmooth_range : public s_twinsmooth {
protected:
    size_t range;

public:
    explicit s_twinsmooth_range(size_t s, size_t r) : s_twinsmooth(s), range(r) {}
    void start() override;
    void execute() override;
};