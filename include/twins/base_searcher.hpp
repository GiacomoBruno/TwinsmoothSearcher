#pragma once
#include "avl_tree.hpp"
#include <vector>
#include <array>
#include <iostream>

namespace twins
{
    class base_searcher
    {
    protected:
        int smoothness{};

        virtual void initialize();
        virtual void run() = 0;
        virtual void dispose();

    public:
        avl_tree *S{nullptr};

        base_searcher(int s) : smoothness{s}, S{new avl_tree} {}

        base_searcher(const base_searcher &) = delete;
        base_searcher(base_searcher &&) = delete;

        virtual ~base_searcher() { dispose(); }

        auto operator=(const base_searcher &) = delete;
        auto operator=(base_searcher &&) = delete;

        void execute()
        {
            initialize();
            run();
        }
    };

    namespace util
    {

        template <class T>
        struct vector_mover
        {
            std::vector<T> *vec;

            vector_mover() : vec{new std::vector<T>} {
                // std::cout << "vector_mover()" << std::endl; 
                }

            vector_mover(const vector_mover<T> &o) : vec{new std::vector<T>(*o.vec)}
            {
                
                //std::cout << "vector_mover(const&)" << std::endl;
            }

            vector_mover(vector_mover<T> &&o) : vec{o.vec}
            {
                o.vec = nullptr;
                //std::cout << "vector_mover(&&)" << std::endl;
            }

            auto operator==(const vector_mover<T>& o){ return o.vec == vec; }

            auto operator!=(const vector_mover<T>& o){ return !(*this == o); }

            auto operator=(const vector_mover<T>& o) 
            {
                //std::cout << "operator=const&" << std::endl;
                if(!(o == *this)) 
                {
                    delete vec;
                    vec = new std::vector<T>(*o.vec);
                }
                return *this;
            }

            auto operator=(vector_mover<T>&& o)
            {
                //std::cout << "operator=&&" << std::endl;
                vec = o.vec;
                o.vec = nullptr;
                return *this;
            }

            ~vector_mover() { delete vec; }
        };

        using batch_t = std::vector<vector_mover<node_iterator<avl_node>>>;

        batch_t* generate_batches(vector_mover<node_iterator<avl_node>> &input);

        vector_mover<node_iterator<avl_node>> generate_twinsmooths(avl_tree *S);
    }


}