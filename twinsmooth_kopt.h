#pragma once
#include <filesystem>
#include "twinsmooth.h"

struct range_pair{
    LinkedList* nodes;
    int lower_bound_distance;
    int upper_bound_distance;
};

class twinsmooth_kopt : public twinsmooth{
protected:
    bigfloat k1;
    bigfloat k2;

    LinkedTree* iteration(LinkedList *pList) override;
    LinkedList* create_chunks(LinkedList* input, int chunk_size) override;

public:
    explicit twinsmooth_kopt(uint64_t s, double _k1, double _k2) : twinsmooth(s) {
        k1 = bigfloat_new;
        k2 = bigfloat_new;

        bigfloat_init(k1, _k1);
        bigfloat_init(k2, _k2);
    }
    void execute() override;
    void terminate() override;
    ~twinsmooth_kopt() override { bigfloat_free(k1); bigfloat_free(k2);};

};

