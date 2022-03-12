#pragma once
#include <cstdint>
#include <filesystem>
#include "twinsmooth.h"


LinkedTree* generate_twinsmooth_from_chunks(std::vector<Node>* chunk);

class twinsmooth_full : public twinsmooth {
protected:
public:
    explicit twinsmooth_full(uint64_t s) : twinsmooth(s) {}
    void execute() override;
    void terminate() override;
    ~twinsmooth_full() override = default;



};
