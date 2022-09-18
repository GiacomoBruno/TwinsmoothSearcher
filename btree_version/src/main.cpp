#include "searcher.hpp"
#include "benchmark.hpp"
#include "range_searcher.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
int main()
{
    benchmark b;

    int s;
    std::cout << "smoothness: ";
    std::cin >> s;
    std::cout << "range: ";
    std::cin >> searcher::RANGE_SIZE;
    std::cout << "k: ";
    std::cin >> searcher::k;
    std::cout << "max bit size: ";
    std::cin >> searcher::MAX_BIT_SIZE;
    std::cout << "max bit size to save: ";
    std::cin >> searcher::MAX_BIT_SIZE_TO_SAVE;

    b.start_bench();

    searcher::PSET res;
    searcher::execute<searcher::range_optimization>(s, res );

    b.conclude_bench();

    std::string output_fd = "./result/";
    std::string output_fl = "twinsmooth.txt";

    std::filesystem::create_directories(output_fd);
    std::fstream file;
    file.open(output_fd + output_fl, std::ios_base::out);

    int smallest_to_save = static_cast<int>(searcher::MAX_BIT_SIZE_TO_SAVE * 0.85);
    int biggest_to_save = static_cast<int>(searcher::MAX_BIT_SIZE_TO_SAVE);
    for(auto x : res)
    {
        int size = mpz_sizeinbase(x->get_mpz_t(), 2);
        if(size >= smallest_to_save && size <= biggest_to_save)
        {
            file << x->get_str() << "\n";
        }
    }

    file.flush();
    file.close();


}