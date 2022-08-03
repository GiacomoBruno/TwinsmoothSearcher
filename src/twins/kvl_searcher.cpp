#include "twins/searchers.hpp"
#include "utilities.hpp"
#include "logger.hpp"
#include <omp.h>
namespace twins
{
    namespace kvl_searcher_util
    {
        template <class T>
        T multiply_double(const T &n, double d) { return static_cast<T>(n * d); }

        template <>
        mpz_class multiply_double<mpz_class>(const mpz_class &n, double d)
        {
            mpf_class _d = d;
            mpf_class _n = n;
            _d *= _n;

            return mpz_class{_d};
        }

        util::vector_mover<mpz_class> generate_twinsmooths(util::vector_mover<node_iterator<avl_node>> &input, avl_tree *S, double k)
        {
            mpz_class d, delta, m1;
            auto x_holder = *input.vec;
            util::vector_mover<mpz_class> output;

            mpz_class num = multiply_double<mpz_class>(*x_holder[0], k);
            int range = x_holder[0] - S->lower_bound(num);
            int counter = 0;

            for (decltype(x_holder.size()) i = 0; i < x_holder.size() && x_holder[i] != nullptr; i++)
            {
                auto x = *x_holder[i];
                auto y_iter = x_holder[i];
                auto z_iter = x_holder[i];

                ++y_iter;
                --z_iter;

                while (y_iter != nullptr && counter < range)
                {
                    counter++;
                    auto y = *y_iter;
                    m1 = x * y;

                    m1 += y;
                    delta = y - x;
                    d = m1 % delta;

                    if (d == 0)
                    {
                        m1 /= delta;
                        m1 -= 1;
                        auto num = mpz_class{m1};
                        auto hint = S->find(num);
                        if (hint == nullptr)
                        {
                            output.vec->push_back(num);
                        }
                    }
                    ++y_iter;
                }
                counter = 0;
                while (z_iter != nullptr && counter < range)
                {
                    counter++;
                    auto z = *z_iter;
                    m1 = x * z;

                    m1 += x;

                    delta = x - z;
                    d = m1 % delta;

                    if (d == 0)
                    {
                        m1 /= delta;
                        m1 -= 1;
                        auto num = mpz_class{m1};
                        auto hint = S->find(num);
                        if (hint == nullptr)
                        {
                            output.vec->push_back(num);
                        }
                    }
                    --z_iter;
                }
            }

            return output;
        }

        void iteration(util::vector_mover<node_iterator<avl_node>> &io, avl_tree *S, double k)
        {
            auto *batches = util::generate_batches(io);
            util::vector_mover<mpz_class> results[batches->size()];

#pragma omp parallel num_threads(utilities::NUM_THREADS)
            {
#pragma omp for
                for (decltype(batches->size()) i = 0; i < batches->size(); i++)
                {
                    results[i] = generate_twinsmooths(batches->at(i), S, k);
                }
            }

            io.vec->clear();

            for (decltype(batches->size()) i = 0; i < batches->size(); i++)
            {
                for (const auto &r : *results[i].vec)
                {
                    auto insertion = S->insert(r);
                    if (insertion != nullptr)
                    {
                        io.vec->push_back(insertion);
                    }
                }
            }
        }
    }

    void kvl_searcher::run()
    {
        auto res = util::generate_twinsmooths(S);

        simple_logger log(smoothness);

        while (!res.vec->empty())
        {
            kvl_searcher_util::iteration(res, S, k);
            log.print("found new twinsmooth: ");
            log.print(res.vec->size());
            log.print("\n");
        }

        log.print("found in total: ");
        log.print(S->size());
        log.print(" smooth numbers\n");
    }

}