#include "twins/base_searcher.hpp"

namespace twins 
{
    namespace util 
    {
        batch_t* generate_batches(vector_mover<node_iterator<avl_node>> &input)
        {
            batch_t* output = new batch_t;

            int chunk_size = 100;
            decltype(input.vec->size()) i = 1;

            for (decltype(input.vec->size()) idx = 0; i <= input.vec->size(); idx++)
            {
                vector_mover<node_iterator<avl_node>> batch;
                for (; i < input.vec->size(); i++)
                {
                    batch.vec->push_back(input.vec->at(i-1));
                    if (i % chunk_size == 0)
                        break;
                }
                output->push_back(batch);
                i++;
            }

            return output;
        }

        vector_mover<node_iterator<avl_node>> generate_twinsmooths(avl_tree *S)
        {
            vector_mover<node_iterator<avl_node>> output;

            mpz_class d, delta, m1;

            auto iter_x = S->begin();

            std::vector<mpz_class> holder;

            while (iter_x != nullptr)
            {
                auto iter_y = iter_x;
                ++iter_y;

                auto x = *iter_x;
                while (iter_y != nullptr)
                {
                    auto y = *iter_y;
                    m1 = x * y;

                    m1 += y;
                    delta = y - x;
                    d = m1 % delta;

                    if (d == 0)
                    {
                        m1 /= delta;
                        m1 -= 1;
                        holder.push_back(mpz_class{m1});
                    }

                    ++iter_y;
                }
                ++iter_x;
            }

            for (auto &&num : holder)
            {
                auto insertion = S->insert(num);
                if (insertion != nullptr)
                {
                    output.vec->push_back(insertion);
                }
            }

            return output;
        }
    }

    void base_searcher::initialize()
    {
        for (int i = 1; i <= smoothness; i++)
        {
            S->insert(mpz_class{i});
        }
    }

    void base_searcher::dispose()
    {
        // here save the results

        delete S;
    }
}