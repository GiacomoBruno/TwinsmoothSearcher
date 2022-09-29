#pragma once
#include "searcher.hpp"

#include <array>

bool is_prime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

std::array<int, 500> PrimeNumbers()
{
    std::array<int, 500> p{};

    p[0] = 2;
    int counter = 1;
    for(int i = 3; counter < 500; i+=2)
    {
        if(is_prime(i))
        {
            p[counter] = i;
            counter++;
        }
    }
    return p;
}

int abs(int n)
{
    return n < 0 ? -n : n;
}

double normalize(double min, double max, int n)
{
    return (n - min) / (max - min);
}

size_t generate_range(int bit_size, size_t smoothness)
{
    static auto primes = PrimeNumbers();

    size_t range = 350;

    int count_primes = 0;

    for(int i = 0; i < primes.size(); i++)
    {
        if(primes[i] <= smoothness)
            count_primes++;
    }

    count_primes -= 16;

    double bit_size_multiplier = 1.0;

    int mid_point = count_primes / 2 + 16;

    bit_size_multiplier = 1.0 - normalize(0.0,mid_point,abs(bit_size - mid_point)) + 0.3;   
    // 0.6 is the minimum multiplier
    // overshoot by 0.6 to make sure that the range is big enough

    for(int i = 0; i < count_primes; i++)
    {
        range += (range * 0.025); //for every prime number add 3% to the range
    }

    //range now is the maximum it can be

    range = range * bit_size_multiplier;

    //std::cout << "range: " << range << std::endl;
    return range < 100 ? 100 : range;

}


namespace searcher {

    struct var_range_optimization {
    };


    template<>
    void generate_twins<var_range_optimization>(std::vector<mpz_class *> &chunk, PSET &S, PSET &output) {
        mpz_class d, delta, m1, result;
        int r = 0;
        for (auto &x: chunk) {
            //forward
            auto y = S.upper_bound(x);
            auto z = S.lower_bound(x);
            auto x_iter = S.find(x);

            size_t RANGE = generate_range(mpz_sizeinbase(x->get_mpz_t(), 2), SMOOTHNESS);

            while (y != S.end() && r < RANGE) {
                m1 = *x * **y;
                m1 += **y;
                delta = **y - *x;
                d = m1 % delta;

                if (d == 0) {
                    m1 /= delta;
                    m1 -= 1;
                    if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= MAX_BIT_SIZE) {
                        auto res = new mpz_class{m1};
                        if (S.find(res) == S.end()) {
                            output.insert(res);
                        } else {
                            delete res;
                        }
                    }
                }
                r++;
                std::advance(y, 1);
            }
            //backward
            r = 0;
            if (z != x_iter)
                while (r < RANGE) {
                    m1 = **z * *x;
                    m1 += *x;
                    delta = *x - **z;
                    d = m1 % delta;

                    if (d == 0) {
                        m1 /= delta;
                        m1 -= 1;

                        if (mpz_sizeinbase(m1.get_mpz_t(), 2) <= MAX_BIT_SIZE) {
                            auto res = new mpz_class{m1};
                            if (S.find(res) == S.end()) {
                                output.insert(res);
                            } else {
                                delete res;
                            }
                        }
                    }
                    r++;
                    if (z == S.begin()) break;
                    std::advance(z, -1);
                }
        }
    }


}