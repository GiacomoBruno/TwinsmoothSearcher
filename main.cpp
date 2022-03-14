#include <chrono>

#include "Tests/test.h"
#include "nb.h"
#include "twinsmooth_range.h"
#include "twinsmooth_full.h"
#include "twinsmooth_kopt.h"


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

uint64_t get_uint64_t(const std::string& s)
{
    std::cout << s;
    std::flush(std::cout);
    uint64_t res;
    std::cin >> res;
    return res;
}

double get_double(const std::string& s)
{
    std::cout << s;
    std::flush(std::cout);
    double res;
    std::cin >> res;
    return res;
}

int main(int argc, char** argv)
{
    auto smoothness = get_uint64_t("choose smoothness: ");
    std::cout << "(0) no optimizations" << std::endl << "(1) range optimization"
    << std::endl << "(2) k optimization" << std::endl;

    uint64_t mode = get_uint64_t("select: ");

    twinsmooth* ts = nullptr;
    while(ts == nullptr)
    {
        switch(mode){
            case 0: ts = new twinsmooth_full(smoothness); break;
            case 1: ts = new twinsmooth_range(smoothness, (int)get_uint64_t("choose range: ")); break;
            case 2: ts = new twinsmooth_kopt(smoothness, get_double("choose k1: "), get_double("choose k2: ")); break;
            default: std::cout << "not a valid option" << std::endl; break;
        }
    }




    start_bench();
    ts->execute();
    ts->terminate();
    delete ts;
    conclude_bench();

    
    return 0;
}


