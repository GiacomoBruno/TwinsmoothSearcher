#include "../include/utilities.h"
#include "../include/benchmark.h"
#include "../include/twinsmooth/all_searchers.h"

int main(int argc, char** argv)
{
    utilities::init_machine();

    searcher_benchmark bench(twinsmooth::get_searcher());

    bench.bench_searcher();

    return 0;

}


