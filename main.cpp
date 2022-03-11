#include <chrono>

#include "nb.h"
#include "twinsmooth_full.h"



#define ELAPSED(START,END) (std::chrono::duration_cast<std::chrono::microseconds>(END - START).count() / (double)1000000)
#define CURRENT_TIME std::chrono::steady_clock::now()
typedef std::chrono::steady_clock::time_point t;



typedef void (*function)();

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


int main(int argc, char** argv)
{
    start_bench();
    uint64_t smoothness = 100;

    twinsmooth_full* tsf = new twinsmooth_full(smoothness);
    tsf->execute();
    tsf->get_results()->cleanup();
    delete tsf;
    conclude_bench();

    
    return 0;
}


