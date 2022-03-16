#pragma once
#include <filesystem>
#include "twinsmooth.h"

struct range_pair {
    LinkedList* nodes;
    Node lower_bound_node;
    Node upper_bound_node;
};

class twinsmooth_kopt : public twinsmooth{
private:

    size_t current_new_results = 0;
    size_t new_found_results = 0;

protected:
    virtual LinkedTree* generate_twinsmooth_from_k_chunks(range_pair* rp);
    LinkedTree* iteration(LinkedList* points);
    LinkedList* create_chunks(LinkedList* input, int chunk_size) override;
    double k1;
    double k2;
    bigfloat current_k = nullptr;
    range_pair* get_range_pair(LinkedList* chunk);

    virtual void select_current_k();
public:
    explicit twinsmooth_kopt(uint64_t s, double _k1, double _k2) : twinsmooth(s) {
        k1 = _k1;
        k2 = _k2;
    }

    void execute() override;
    void terminate() override;
    ~twinsmooth_kopt() override = default;;

};

