#pragma once
#include <filesystem>
#include "twinsmooth.h"

struct range_pair{
    LinkedList* nodes;
    size_t lower_bound_distance;
    size_t upper_bound_distance;
};

class twinsmooth_kopt : public twinsmooth{
private:
    bigfloat k1 = nullptr;
    bigfloat k2 = nullptr;
    size_t current_new_results = 0;
    size_t new_found_results = 0;

protected:
    virtual LinkedTree* generate_twinsmooth_from_k_chunks(range_pair* rp);
    LinkedTree* iteration(LinkedList* points) override;
    LinkedList* create_chunks(LinkedList* input, int chunk_size) override;
    bigfloat current_k = nullptr;

    virtual void select_current_k();
    explicit twinsmooth_kopt(uint64_t s) : twinsmooth(s){
        k1 = bigfloat_new;
        k2 = bigfloat_new;
        bigfloat_init(k1, 0);
        bigfloat_init(k2, 0);
    };

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

