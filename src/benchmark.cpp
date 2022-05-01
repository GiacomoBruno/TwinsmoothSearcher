//
// Created by Giacomo Bruno on 30/04/22.
//

#include "../include/benchmark.h"
#include "../include/utilities.h"

using namespace utilities;

void benchmark::start_bench()
{
    start_time = std::chrono::steady_clock::now();
}

void benchmark::conclude_bench()
{
    end_time = std::chrono::steady_clock::now();
    LG->log("seconds: ", ((double)std::chrono::duration_cast<std::chrono::microseconds>((end_time) - (start_time)).count() / (double) 1000000));
}

void searcher_benchmark::bench_searcher() {
    start_bench();
    app->execute();
    LG->log("benchmark completed in ");
    conclude_bench();
}
