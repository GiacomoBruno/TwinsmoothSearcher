#pragma once
#include <chrono>
#include "twinsmooth/searcher.h"

class benchmark {
private:
    using t = std::chrono::steady_clock::time_point;

    t start_time{};
    t end_time{};

public:
    void start_bench();
    void conclude_bench();

};

class searcher_benchmark : public benchmark {
private:
    twinsmooth::searcher* app{nullptr};
public:
    explicit searcher_benchmark(twinsmooth::searcher* s) : app(s) {}

    void bench_searcher();
};


