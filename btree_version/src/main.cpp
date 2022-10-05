#include "searcher.hpp"
#include "benchmark.hpp"
#include "range_searcher.hpp"
#include "k_searcher.hpp"
#include "variable_range_searcher.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <array>
#include <tuple>
#include <vector>
//each prime added to the initial set, increases the max range size by 20%
//on average the bitsize with the largest range increases by 1 with every second prime added


//if we start with with a range of 300 at prime number 53 with bitsize medium = 15

//at prime number 997 we should have 123 increases of 20% over the initial 300 and bitsize medium increased by 61.5
//bitsize medium is probably a logaritmic increas and not a linear one

//360->

bool isprime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true; //if both failed then num is prime
}

std::array<int, 1000> primes()
{
    std::array<int, 1000> _primes{};

    _primes[0] = 2;
    int counter = 1;

    for (int i = 3; counter < 1000 ; i += 2)
    {
        if (isprime(i))
        {
            _primes[counter] = i;
            counter++;
        }
    }

    return _primes;
}

int countPrimes(int strt, int end) {
    int count = 0;
    for (int i = strt;i <= end;i++) {
        if (isprime(i) == 1)
        {
            count++;
        }
    }
    return count;
}


void read_parameters()
{
    std::cout << "smoothness: ";
    std::cin >> GLOBALS.Smoothness;

    std::cout << "optimizations available: ";
    std::cout << "\n\t" << "no optimizations: " << static_cast<int>(globals::OPTIMIZATION_LEVELS::NO_OPTIMIZATION);
    std::cout << "\n\t" << "constant range:   " << static_cast<int>(globals::OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION);
    std::cout << "\n\t" << "global k:         " << static_cast<int>(globals::OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION);
    std::cout << "\n\t" << "iterative k:      " << static_cast<int>(globals::OPTIMIZATION_LEVELS::ITERATIVE_K_OPTIMIZATION);
    std::cout << "\n\t" << "iterative range:  " << static_cast<int>(globals::OPTIMIZATION_LEVELS::ITERATIVE_RANGE_OPTIMIZATION);
    std::cout << "\n\t" << "variable range:   " << static_cast<int>(globals::OPTIMIZATION_LEVELS::VARIABLE_RANGE_OPTIMIZATION);
    std::cout << "\nchoose: ";
    std::cin >> GLOBALS.OptimizationType;

    switch(GLOBALS.OptimizationType)
    {
        case 0: break;
        case 1:
            std::cout << "range: ";
            std::cin >> GLOBALS.RangeCurrent;
            break;
        case 2:
            std::cout << "k: ";
            std::cin >> GLOBALS.KCurrent;
            break;
    }

    bool bit_size_opt =false;
    std::cout << "Ignore big numbers? (0 = no, 1 = yes): ";
    std::cin >> bit_size_opt;

    if(bit_size_opt)
    {
        std::cout << "max bit size: ";
        std::cin >> GLOBALS.MaxBitSize;
    }

    bool bit_size_save =false;

    std::cout << "Save only some numbers close to N bit length? (0 = no, 1 = yes): ";
    std::cin >> bit_size_save;

    if(bit_size_save) {
        std::cout << "max bit size to save: ";
        std::cin >> GLOBALS.MaxBitSizeToSave;
        std::cout << "min bit size to save: ";
        std::cin >> GLOBALS.MinBitSizeToSave;
    }

    std::cout << "output file name: ";
    std::cin >> GLOBALS.OutputFile;
}

int main()
{
    benchmark b;


    read_parameters();




    searcher::PSET res;
        b.start_bench();
        using O = globals::OPTIMIZATION_LEVELS;
        switch (static_cast<O>(GLOBALS.OptimizationType)) {
            case O::NO_OPTIMIZATION:
                searcher::execute<O::NO_OPTIMIZATION>(res);
                break;
            case O::CONSTANT_RANGE_OPTIMIZATION:
                searcher::execute<O::CONSTANT_RANGE_OPTIMIZATION>(res);
                break;
            case O::GLOBAL_K_OPTIMIZATION:
                searcher::execute<O::GLOBAL_K_OPTIMIZATION>(res);
                break;
            case O::VARIABLE_RANGE_OPTIMIZATION:
                searcher::execute<O::VARIABLE_RANGE_OPTIMIZATION>(res);
            default:
                break;
        }
        b.conclude_bench();

    std::string output_fd = "./result/";
    std::string output_fl = GLOBALS.OutputFile;

    std::filesystem::create_directories(output_fd);
    std::fstream file;
    file.open(output_fd + output_fl, std::ios_base::out);

    if(GLOBALS.MaxBitSizeToSave == 1024)
    {
        for(auto x : res)
        {
            file << x->get_str() << "\n";
        }
    }
    else {

        int smallest_to_save = static_cast<int>(GLOBALS.MinBitSizeToSave);
        int biggest_to_save = static_cast<int>(GLOBALS.MaxBitSizeToSave);
        for (auto x: res) {
            int size = mpz_sizeinbase(x->get_mpz_t(), 2);
            if (size >= smallest_to_save && size <= biggest_to_save) {
                file << x->get_str() << "\n";
            }
        }
    }

    file.flush();
    file.close();


}