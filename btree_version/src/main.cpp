#include "searcher.hpp"
#include "benchmark.hpp"
#include "range_searcher.hpp"
#include "k_searcher.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

void read_parameters()
{
    std::cout << "optimization [0 = none, 1 = range, 2 = k]: ";
    std::cin >> searcher::OPTIMIZATION;

    switch(searcher::OPTIMIZATION)
    {
        case 0: break;
        case 1:
            std::cout << "range: ";
            std::cin >> searcher::RANGE_SIZE;
            break;
        case 2:
            std::cout << "k: ";
            std::cin >> searcher::k;
            break;
    }

    bool bit_size_opt =false;
    std::cout << "Ignore big numbers? (0 = no, 1 = yes): ";
    std::cin >> bit_size_opt;

    if(bit_size_opt)
    {
        std::cout << "max bit size: ";
        std::cin >> searcher::MAX_BIT_SIZE;
    }

    bool bit_size_save =false;

    std::cout << "Save only some numbers close to N bit length? (0 = no, 1 = yes): ";
    std::cin >> bit_size_save;

    if(bit_size_save) {
        std::cout << "max bit size to save: ";
        std::cin >> searcher::MAX_BIT_SIZE_TO_SAVE;
        std::cout << "min bit size to save: ";
        std::cin >> searcher::MIN_BIT_SIZE_TO_SAVE;
    }

    std::cout << "output file name: ";
    std::cin >> searcher::output_file;
}

int main()
{
    benchmark b;

    int s;
    std::cout << "smoothness: ";
    std::cin >> s;
    read_parameters();

    b.start_bench();

    searcher::PSET res;
    switch(searcher::OPTIMIZATION)
    {
        case 0:

            searcher::execute<searcher::no_optimization>(s, res);
            break;
        case 1:
            searcher::execute<searcher::range_optimization>(s,res);
            break;
        case 2:
            searcher::execute<searcher::k_optimization>(s,res);
            break;
        default: break;
    }

    b.conclude_bench();

    std::string output_fd = "./result/";
    std::string output_fl = searcher::output_file;

    std::filesystem::create_directories(output_fd);
    std::fstream file;
    file.open(output_fd + output_fl, std::ios_base::out);

    if(searcher::MAX_BIT_SIZE_TO_SAVE == 1024)
    {
        for(auto x : res)
        {
            file << x->get_str() << "\n";
        }
    }
    else {

        int smallest_to_save = static_cast<int>(searcher::MIN_BIT_SIZE_TO_SAVE);
        int biggest_to_save = static_cast<int>(searcher::MAX_BIT_SIZE_TO_SAVE);
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