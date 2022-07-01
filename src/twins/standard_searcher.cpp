#include "twins/searchers.hpp"
#include <omp.h>
namespace twins
{
    namespace std_searcher_util
    {
        util::vector_mover<mpz_class> generate_twinsmooths(util::vector_mover<node_iterator<avl_node>> &input, avl_tree *S)
        {
            mpz_class d, delta, m1;
            auto x_holder = *input.vec;
            util::vector_mover<mpz_class> output;

            for (decltype(x_holder.size()) i = 0; i < x_holder.size() && x_holder[i] != nullptr; i++)
            {
                auto x = *x_holder[i];
                auto y_iter = x_holder[i];
                auto z_iter = x_holder[i];

                ++y_iter;
                --z_iter;

                while (y_iter != nullptr)
                {
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

                while (z_iter != nullptr)
                {
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

        void iteration(util::vector_mover<node_iterator<avl_node>> &io, avl_tree *S)
        {
            auto *batches = util::generate_batches(io);
            util::vector_mover<mpz_class> results[batches->size()];

#pragma omp parallel num_threads(12)
            {
#pragma omp for
                for (decltype(batches->size()) i = 0; i < batches->size(); i++)
                {
                    results[i] = generate_twinsmooths(batches->at(i), S);
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

    void std_searcher::run()
    {
        auto res = util::generate_twinsmooths(S);

        while(!res.vec->empty())
        {
            std_searcher_util::iteration(res, S);
            std::cout << "found new twinsmooth: " << res.vec->size() << std::endl; 
        }
    }

}