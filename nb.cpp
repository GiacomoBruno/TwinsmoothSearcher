#include "nb.h"

t start_time, end_time;
clock_t st, et;


void start_bench()
{
start_time = CURRENT_TIME;
st = clock();
}

void conclude_bench()
{
et = clock();
end_time = CURRENT_TIME;

printf("Wall time: %f\n", ELAPSED(start_time, end_time) );
printf("Clock time: %f\n", (double)(et-st)/CLOCKS_PER_SEC);
}