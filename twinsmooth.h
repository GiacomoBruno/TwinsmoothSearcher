#pragma once

#include "nb.h"
#include "tree.h"


class twinsmooth
{
protected:
    const uint64_t smoothness;

    void load_files();
    void save_files();
    virtual LinkedList* create_chunks(LinkedList* input, int chunk_size);
    virtual LinkedTree* iteration(LinkedList* points) = 0;



    LinkedTree *results;
    LinkedList *computation_numbers;

public:


    explicit twinsmooth(uint64_t s) : smoothness(s), results(new LinkedTree()), computation_numbers(new LinkedList()) { }
    virtual ~twinsmooth() = default;
    [[nodiscard]] LinkedTree* get_results() const { return results; }
    virtual void execute() = 0;
    virtual void terminate() = 0;
};
