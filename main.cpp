#include <chrono>

#include "nb.h"
#include "twinsmooth_range.h"
#include "twinsmooth_k.h"
#include "twinsmooth_k_growing.h"
#include "prime_calculator.h"
#include "prime_calculator_top_n.h"
#include "twinsmooth_test.h"

bool get_bool(const std::string& s)
{
    std::cout << s;
    std::flush(std::cout);
    bool res;
    std::cin >> res;
    return res;
}

size_t get_size_t(const std::string& s)
{
    std::cout << s;
    std::flush(std::cout);
    size_t res;
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
    std::cout << sizeof(struct node) << std::endl;
    auto smoothness = get_size_t("choose smoothness: ");
    lg = new logger(STATUS_FN, OUT_FOLDER(smoothness), smoothness);

    std::cout << "(0) no optimizations" <<
    std::endl << "(1) range optimization" <<
    std::endl << "(2) k optimization" <<
    std::endl << "(3) increasing k optimization" <<
    std::endl << "(4) calculate primes" <<
    std::endl << "(5) calculate top primes" <<
    std::endl << "(6) test" <<
    std::endl;
    size_t mode = get_size_t("select: ");
    s_twinsmooth* s = nullptr;
    std::filesystem::create_directories(OUT_FOLDER(smoothness));

    while(s == nullptr)
    {
        switch(mode){
            case 0: s = new s_twinsmooth(smoothness); break;
            case 1: s = new s_twinsmooth_range(smoothness, (int)get_size_t("choose range: ")); break;
            case 2: s = new s_twinsmooth_k(smoothness, get_double("choose k: ")); break;
            case 3: s = new s_twinsmooth_k_growing(smoothness, get_double("chose start k: "), get_double("chose max k: "),get_double("chose k step: "),
                                                   get_size_t("amount of top twins to log per K: ")); break;
            case 4: s = new prime_calculator(smoothness); break;
            case 5: s = new prime_calculator_top_n(smoothness, (int)get_size_t("choose number of primes to look for: "), get_bool("is the set of twins in reverse order? ")); break;
            case 6: s = new twinsmooth_test(smoothness); break;
            default: std::cout << "not a valid option" << std::endl; break;
        }
    }
   benchmark b;
   s->start();
   b.start_bench();
   s->execute();
   b.conclude_bench();
   s->terminate();

   /*
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
   b.conclude_bench();*/



    return 0;
}


