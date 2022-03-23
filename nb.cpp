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
    lg->log("time in seconds : ", ELAPSED(start_time, end_time));
    lg->logl(" - CPU time in seconds: ", (double)(et-st)/CLOCKS_PER_SEC);
}