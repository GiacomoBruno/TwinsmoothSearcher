//
// Created by Giacomo Bruno on 23/03/22.
//

#pragma once
#include "nb.h"
#include "tree.h"

class static_twinsmooth {
private:
    static LinkedList* create_chunks(LinkedList *input, size_t chunk_size);
    static LinkedList* create_chunks(LinkedTree* input, size_t chunk_size);

    static void k_get_ranges(Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, size_t& lower_skip, size_t& upper_skip, bigfloat k, bigfloat oldk);
    static void k_get_ranges(Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, bigfloat k);

    //No optimization twinsmooth generation
    static LinkedTree* generate_twinsmooth(LinkedList* chunk);

    static LinkedTree* r_generate_twinsmooth(LinkedList* chunk,  size_t range);
    static LinkedTree* r_generate_twinsmooth(LinkedList* chunk, size_t range, size_t skip_range);

    static LinkedTree* k_generate_twinsmooth(LinkedList *chunk, LinkedTree *S, bigfloat k);
    static LinkedTree* k_generate_twinsmooth(LinkedList *chunk, LinkedTree *S, bigfloat k, bigfloat oldk);


public:
    static LinkedTree* k_iteration_S_S(LinkedTree* S, bigfloat k, bigfloat oldk);
    static LinkedTree* k_iteration_S_S(LinkedTree* S, bigfloat k);
    static LinkedTree* k_iteration_S_N(LinkedTree* S, LinkedList* N, bigfloat k);

    //No optimization SxN single iteration
    static LinkedTree* iteration_S_N(LinkedList* N);
    static LinkedTree* iteration_S_S(LinkedTree* S);

    //range optimization SxN single iteration
    static LinkedTree* r_iteration_S_S(LinkedTree* S, size_t range);
    static LinkedTree* r_iteration_S_S(LinkedTree* S, size_t range, size_t skip_range);
    static LinkedTree* r_iteration_S_N(LinkedList* N, size_t range);
    static LinkedTree* r_iteration_S_N(LinkedTree* S, LinkedTree* N, size_t range, size_t skip_range);



};