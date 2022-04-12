//
// Created by Giacomo Bruno on 23/03/22.
//

#pragma once
#include "nb.h"
#include "tree.h"

class static_twinsmooth {
private:
    static LinkedList<Node*>* create_chunks(LinkedList<Node> *input);
    static LinkedList<Node*>* create_chunks(LinkedTree* input);


    static void k_get_ranges(Node nd, LinkedTree *results, size_t &lower_range, size_t &upper_range, size_t &lower_skip,
                             size_t &upper_skip, bigfloat k, bigfloat oldk);

    static void k_get_ranges(Node nd, LinkedTree *results, size_t &lower_range, size_t &upper_range, bigfloat k);

    //No optimization twinsmooth generation
    static LinkedList<bigint>* generate_twinsmooth(Node* chunk, LinkedTree* S);

    static LinkedList<bigint>* r_generate_twinsmooth(Node* chunk, LinkedTree* S, size_t range);

    static LinkedList<bigint>* r_generate_twinsmooth(Node* chunk, LinkedTree* S, size_t range, size_t old_range);

    static LinkedList<bigint>* k_generate_twinsmooth(Node* chunk, LinkedTree *S, bigfloat k);

    static LinkedList<bigint>* k_generate_twinsmooth(Node* chunk, LinkedTree *S, bigfloat k, bigfloat oldk);
    //prime calculation
    static LinkedList<bigint>* generate_primes(LinkedList<bigint*>* chunk);

public:
    static LinkedList<Node>* k_iteration_S_S(LinkedTree *S, bigfloat k, bigfloat oldk);

    static LinkedList<Node>* k_iteration_S_S(LinkedTree *S, bigfloat k);

    static LinkedList<Node>* k_iteration_S_N(LinkedTree *S, LinkedList<Node> *N, bigfloat k);

    //No optimization SxN single iteration
    static LinkedList<Node>* iteration_S_N(LinkedTree* S, LinkedList<Node> *N);

    static LinkedList<Node>* iteration_S_S(LinkedTree *S);

    //range optimization SxN single iteration
    static LinkedList<Node>* r_iteration_S_S(LinkedTree *S, size_t range);

    static LinkedList<Node>* r_iteration_S_S(LinkedTree* S, size_t range, size_t old_range);

    static LinkedList<Node>* r_iteration_S_N(LinkedTree *S, LinkedList<Node> *N, size_t range);


};