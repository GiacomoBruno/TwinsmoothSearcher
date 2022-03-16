//
// Created by Giacomo Bruno on 16/03/22.
//

#pragma once
#include "twinsmooth.h"

struct chunk{
    LinkedList* nodes;
    Node lower_bound;
    Node lower_start;
    Node upper_bound;
    Node upper_start;
};

class twinsmooth_k : public twinsmooth {
    bigfloat k = nullptr;
    chunk* create_chunk(LinkedList* chnk, double small_k);

    LinkedList* create_chunks_from_tree(size_t chunk_size, double small_k);
    LinkedList* create_chunks_from_list(LinkedList* input, size_t chunk_size);

public:

    void set_k(double _k);

    explicit twinsmooth_k(uint64_t s) : twinsmooth(s) {}


    LinkedTree* iteration_S_S_no_small_k(double small_k);
    LinkedTree* iteration_S_S();
    LinkedTree* iteration_S_N(LinkedList* input);

    void execute() override {};
    void terminate() override {};

};