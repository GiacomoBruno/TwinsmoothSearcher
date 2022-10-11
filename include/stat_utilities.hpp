#pragma once

#include "global_definitions.hpp"

#include <iostream>
#include <map>

namespace searcher {
using namespace globals;

void PrintBitsizeDistribution(PSET &S)
{
    std::map<int, int> bitsizes{};

    for (auto n : S) {
        int size = mpz_sizeinbase(n->get_mpz_t(), 2);

        if (bitsizes.find(size) == bitsizes.end())
            bitsizes[size] = 1;
        else
            bitsizes[size] += 1;
    }

    std::cout << "BITS->     AMOUNT\n";

    for (auto bsize : bitsizes) {
        std::cout << std::setw(3) << bsize.first << " -> " << std::setw(10)
                  << bsize.second << std::endl;
    }
}

std::map<size_t, size_t> GetKDistanceByBitsize(PSET &S)
{
    std::map<size_t, std::vector<size_t>> allMap;

    for (auto x : S) {
        mpz_class low = *x / 2;
        mpz_class high = *x * 2;

        auto low_iter = S.upper_bound(&low);
        auto high_iter = S.lower_bound((&high));

        size_t counter = 0;
        auto x_iter = S.find(x);
        auto iter = x_iter;
        while (iter != S.end() && iter != low_iter) {
            counter++;
            std::advance(iter, -1);
        }
        iter = x_iter;
        while (iter != S.end() && iter != high_iter) {
            counter++;
            std::advance(iter, 1);
        }

        auto bitsize = mpz_sizeinbase(x->get_mpz_t(), 2);

        if (allMap.find(bitsize) == allMap.end())
            allMap[bitsize] = {};

        allMap[bitsize].push_back(counter);
    }
    std::map<size_t, size_t> result{};

    for (const auto &bitsize : allMap) {
        size_t average = 0;
        for (auto count : bitsize.second)
            average += count;

        average = average / bitsize.second.size();
        result[bitsize.first] = average;
    }

    for (size_t i = 0; i < 80; i++) {
        if (result.find(i) == result.end())
            result[i] = 0;
    }

    return result;
}

} // namespace searcher