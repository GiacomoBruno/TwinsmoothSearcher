#include "benchmark.hpp"
#include <iostream>


void benchmark::start_bench()
{
    start_time = std::chrono::steady_clock::now();
}

void benchmark::conclude_bench()
{
    end_time = std::chrono::steady_clock::now();
    std::cout << "seconds: " << ((double)std::chrono::duration_cast<std::chrono::microseconds>((end_time) - (start_time)).count() / (double) 1000000);
}
