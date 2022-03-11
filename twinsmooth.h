#pragma once

#include "nb.h"
#include "tree.h"

LinkedTree* generate_twinsmooth_complete(Node start_number);
LinkedList* create_chunks(LinkedList* input, int chunk_size);

class twinsmooth
{
protected:
    const uint64_t smoothness;

    virtual void initialize_smooth_set() = 0;
    virtual void load_files() = 0;

    LinkedTree *results;

public:


    explicit twinsmooth(uint64_t s) : smoothness(s), results(new LinkedTree()) { }
    ~twinsmooth()= default;
    [[nodiscard]] LinkedTree* get_results() const { return results; }
    virtual void execute() = 0;
    virtual void terminate() = 0;
};
