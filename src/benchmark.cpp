#include "benchmark.hpp"

using namespace std::chrono;

void benchmark::start_bench()
{
    start_time = steady_clock::now();
}

void benchmark::conclude_bench()
{
    end_time = steady_clock::now();
}

double benchmark::seconds_passed() const
{
    return (
        static_cast<double>(
            duration_cast<microseconds>((end_time) - (start_time)).count()) /
        1000000.0);
}