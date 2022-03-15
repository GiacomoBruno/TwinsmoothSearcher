//
// Created by Giacomo Bruno on 15/03/22.
//

#pragma once
#include "twinsmooth_kopt.h"

class twinsmooth_growing_k : public twinsmooth_kopt {
protected:
    double k_start;
    double k_end;
    double k_step;
    double k_current;

    void select_current_k() override;
    LinkedTree* generate_twinsmooth_from_k_chunks(range_pair* rp) override;
    LinkedList* create_chunks(LinkedList* input, int chunk_size) override;
    LinkedList* create_chunks_from_tree(LinkedTree* input, int chunk_size);


public:
    explicit twinsmooth_growing_k(uint64_t s1, double _kstart, double _kend, double _kstep) : twinsmooth_kopt(s1) {
            k_start = _kstart;
            k_end = _kend;
            k_step = _kstep;
            k_current = _kstart-k_step;
            current_k = nullptr;
    }
    //void execute() override;
    //void terminate() override;
    ~twinsmooth_growing_k() override { bigfloat_free(current_k); }

    void execute() override;
    LinkedTree* iteration(LinkedList *points) override;

};


