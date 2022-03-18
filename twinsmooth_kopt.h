#pragma once
#include <filesystem>
#include "twinsmooth.h"

class twinsmooth_kopt : public twinsmooth {
private:

protected:
    virtual LinkedTree* generate_twinsmooth_from_k_chunks(LinkedList* chunk);
    LinkedTree* iteration(LinkedList* points);
    bigfloat k;
    double dk;
    void get_ranges(Node nd, size_t& lower_range, size_t& upper_range);

public:
    explicit twinsmooth_kopt(uint64_t s, double _k) : twinsmooth(s) {
        dk = _k;
        k = bigfloat_new;
        bigfloat_init(k, _k);
    }

    void execute() override;
    void terminate() override;
    ~twinsmooth_kopt() override { bigfloat_free(k); };
};

