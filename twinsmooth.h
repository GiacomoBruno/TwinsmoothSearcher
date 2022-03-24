#pragma once

#include "nb.h"
#include "tree.h"
#include "file_manager.h"


class s_twinsmooth{
protected:
    size_t smoothness;
    LinkedTree* S;
    LinkedList* N;
    CappedFile output_file;

    virtual void load_files();
    void init_set();

public:
    explicit s_twinsmooth(size_t s) : smoothness(s), S(new LinkedTree), N(new LinkedList) {}
    virtual void execute();
    virtual void terminate();
    virtual void start();
};