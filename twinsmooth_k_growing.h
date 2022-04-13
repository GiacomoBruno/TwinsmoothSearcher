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
    size_t amount_of_top_twins_to_log;
    uint16_t threshold;

    void print_top_numbers();
    void update_threshold(LinkedList<Node>* nodes);
    void update_threshold();
    void get_good_twins();

    void increment_k();
public:
    s_twinsmooth_k_growing(size_t s, double sk, double ek, double ks, size_t top_to_save);
    void start() override;
    void execute() override;
    void terminate() override;
};