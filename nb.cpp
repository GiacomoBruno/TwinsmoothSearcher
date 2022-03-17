#include "nb.h"




void benchmark::start_bench()
{
    start_time = CURRENT_TIME;
    st = clock();
}

void benchmark::conclude_bench()
{
    et = clock();
    end_time = CURRENT_TIME;
    printf("Time: %f - CPU Time: %f\n", ELAPSED(start_time, end_time), (double)(et-st)/CLOCKS_PER_SEC );
}