#pragma once

#include "nb.h"
#include "tree.h"


class twinsmooth
{
protected:
    const uint64_t smoothness;


    virtual LinkedList* create_chunks(LinkedList* input, int chunk_size);
    //static LinkedTree* iteration(LinkedList* points);
    static LinkedTree* generate_twinsmooth(LinkedList* input);

    LinkedTree *results;
    LinkedList *computation_numbers;

public:
    [[nodiscard]] size_t get_size_iteration_result() const { return computation_numbers->size(); }
    LinkedTree* get_results() { return results; }

    explicit twinsmooth(uint64_t s) : smoothness(s), results(new LinkedTree()), computation_numbers(new LinkedList()) { }
    virtual ~twinsmooth() = default;
    [[nodiscard]] LinkedTree* get_results() const { return results; }
    virtual void execute() = 0;
    virtual void terminate() = 0;
    virtual void start();
    void load_files();
    void init_starting_set();
    void save_files();

};
