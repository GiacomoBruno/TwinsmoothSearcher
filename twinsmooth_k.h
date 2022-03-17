//
// Created by Giacomo Bruno on 16/03/22.
//

#pragma once
#include "twinsmooth.h"

class twinsmooth_k {
private:
    static LinkedTree* generate_twinsmooth(LinkedTree *S, LinkedList *input, bigfloat k);
    static LinkedTree* generate_twinsmooth_no_oldk(LinkedTree *S, LinkedList *input, bigfloat k, bigfloat oldk);
    static LinkedList *create_chunks(LinkedList *input, size_t chunk_size);
    static LinkedList *create_chunks(LinkedTree* input, size_t chunk_size);
    static void get_ranges(Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, size_t& lower_skip, size_t& upper_skip, bigfloat k, bigfloat oldk);
    static void get_ranges(Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, bigfloat k);
public:
    static LinkedTree* iteration_S_S_no_oldk(LinkedTree* S, bigfloat k, bigfloat oldk);
    static LinkedTree* iteration_S_S(LinkedTree* S, bigfloat k);
    static LinkedTree* iteration_S_N(LinkedTree* S, LinkedList* N, bigfloat k);



    };