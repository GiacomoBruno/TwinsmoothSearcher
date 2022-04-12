#pragma once
#include "twinsmooth.h"

class s_twinsmooth_range_growing : public s_twinsmooth {
protected:

    size_t start_range;
    size_t end_range;
    size_t step_range;

    size_t current_range;
    size_t amount_of_top_twins_to_log;

    void print_top_numbers();

    void increment_range();
public:
    s_twinsmooth_range_growing(size_t s,size_t srange, size_t erange, size_t step, size_t top_to_save);
    void start() override;
    void execute() override;
    void terminate() override;
};