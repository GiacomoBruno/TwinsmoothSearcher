#pragma once
#include "twinsmooth.h"


LinkedTree* generate_twinsmooth_from_chunks(LinkedList* chunk, int range);

class twinsmooth_range : public twinsmooth {
protected:
    int range = 0;
    LinkedTree* iteration(LinkedList* points) override;

public:
    explicit twinsmooth_range(uint64_t s, int _range) : twinsmooth(s), range(_range) {}
    void execute() override;
    void terminate() override;
    ~twinsmooth_range() override = default;



};