#include <chrono>

#include "nb.h"
#include "twinsmooth_range.h"
#include "twinsmooth_full.h"
#include "twinsmooth_kopt.h"
#include "twinsmooth_k_growing.h"

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
    std::cout << "(0) no optimizations" <<
    std::endl << "(1) range optimization" <<
    std::endl << "(2) k optimization" <<
    std::endl << "(3) increasing k optimization" <<
    std::endl;

    uint64_t mode = get_uint64_t("select: ");

    twinsmooth* ts = nullptr;
    while(ts == nullptr)
    {
        switch(mode){
            case 0: ts = new twinsmooth_full(smoothness); break;
            case 1: ts = new twinsmooth_range(smoothness, (int)get_uint64_t("choose range: ")); break;
            case 2: ts = new twinsmooth_kopt(smoothness, get_double("choose k: ")); break;
            case 3: ts = new twinsmooth_k_growing(smoothness, get_double("chose start k: "), get_double("chose max k: "),get_double("chose k step: ")); break;
            default: std::cout << "not a valid option" << std::endl; break;
        }
    }
    lg = new logger(STATUS_FN, OUT_FOLDER(smoothness), smoothness);


    //twinsmooth* ts = new twinsmooth_range(150,100);
    auto b = benchmark();
    b.start_bench();
    ts->start();
    ts->execute();
    ts->terminate();
    delete ts;
    b.conclude_bench();



    return 0;
}


