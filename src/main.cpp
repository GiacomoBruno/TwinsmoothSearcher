#include "../include/utilities.h"
#include "../include/benchmark.h"
#include "../include/twinsmooth/all_searchers.h"
#include <random>
int main(int argc, char** argv)
{
    utilities::init_machine();



    searcher_benchmark bench(twinsmooth::get_searcher());

    bench.bench_searcher();

    //auto s = twinsmooth::get_searcher();
    //s->execute();


    return 0;

}


