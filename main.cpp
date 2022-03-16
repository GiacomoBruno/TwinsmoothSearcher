#include <chrono>

#include "Tests/test.h"
#include "nb.h"
#include "twinsmooth_range.h"
#include "twinsmooth_full.h"
#include "twinsmooth_kopt.h"
#include "twinsmooth_k_growing.h"
//#include "twinsmooth_growing_k.h"


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
/*
 * k = 1.5    computations 1631711729 = 77 seconds  tot res = 341915 tot
 * k = 1.4    computations 1340528651 = 61 seconds  tot res = 340380 tot
 * k = 1.3    computations 1019455851 = 47 seconds  tot res = 334773 tot
21924074488099699579224
23867278858104945657855
24727786361863334229375
106718270337850937604174
151708612973105285459999
178240123880049654296874
302554740285721313106024
589864439608716991201560
 * k = 1.2    computations  671392366 = 30 seconds  tot res = 321499 tot
biggest results
21924074488099699579224
23867278858104945657855
24727786361863334229375
25450316056074220028640
106718270337850937604174
151708612973105285459999
178240123880049654296874
302554740285721313106024
589864439608716991201560
 * k = 1.1    computations  148241482 =  6 seconds  tot res = 200464 tot
biggest results
16952992710360863704064
21924074488099699579224
23867278858104945657855
24727786361863334229375
25450316056074220028640
302554740285721313106024

 */

    auto smoothness = get_uint64_t("choose smoothness: ");
    std::cout << "(0) no optimizations" <<
    std::endl << "(1) range optimization" <<
    std::endl << "(2) k optimization" <<
    //std::endl << "(3) increasing k optimization" <<
    std::endl;

    uint64_t mode = get_uint64_t("select: ");

    twinsmooth* ts = nullptr;
    while(ts == nullptr)
    {
        switch(mode){
            case 0: ts = new twinsmooth_full(smoothness); break;
            case 1: ts = new twinsmooth_range(smoothness, (int)get_uint64_t("choose range: ")); break;
            case 2: ts = new twinsmooth_kopt(smoothness, get_double("choose k: ")); break;
            //case 3: ts = new twinsmooth_growing_k(smoothness, get_double("chose start k: "), get_double("chose max k: "),get_double("chose k step: "));
            default: std::cout << "not a valid option" << std::endl; break;
        }
    }



    //twinsmooth* ts = new twinsmooth_range(150,100);
    start_bench();
    ts->start();
    ts->execute();
    ts->terminate();
    delete ts;
    conclude_bench();



    return 0;
}


