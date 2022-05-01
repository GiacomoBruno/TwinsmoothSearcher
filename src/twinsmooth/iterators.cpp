//
// Created by Giacomo Bruno on 21/04/22.
//

#include "../../include/twinsmooth/iterators.h"
#include "../../include/utilities.h"
#include "../../include/twinsmooth/chunkers.h"
#include "../../include/twinsmooth/generators.h"

#include <omp.h>

using namespace utilities;

namespace twinsmooth {

    LinkedList<TreeNode> *iteration_S_S(bigint_tree *S) {
        auto chunks = create_chunks(S);
        auto new_N = new LinkedList<TreeNode>();

        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if ((found_numbers)[i] != nullptr) {
                    new_N->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return new_N;
    }

    LinkedList<TreeNode> *iteration_S_N(bigint_tree *S, LinkedList<TreeNode> *N) {
        auto chunks = create_chunks(N);
        auto result = new LinkedList<TreeNode>();

        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if (found_numbers[i] != nullptr) {
                    result->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return result;
    }


    LinkedList<TreeNode>* iteration_S_S(bigint_tree *S, int range)
    {
        auto chunks = create_chunks(S);
        auto new_N = new LinkedList<TreeNode>();

        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S, range);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if ((found_numbers)[i] != nullptr) {
                    new_N->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return new_N;
    }

    LinkedList<TreeNode>* iteration_S_N(bigint_tree *S, LinkedList<TreeNode> *N, int range)
    {
        auto chunks = create_chunks(N);
        auto result = new LinkedList<TreeNode>();

        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S, range);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if (found_numbers[i] != nullptr) {
                    result->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return result;
    }


    LinkedList<TreeNode>* iteration_S_S(bigint_tree *S, double k)
    {

        auto chunks = create_chunks(S);
        auto new_N = new LinkedList<TreeNode>();
        bigfloat K{k};
        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S, K);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if ((found_numbers)[i] != nullptr) {
                    new_N->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return new_N;
    }

    LinkedList<TreeNode>* iteration_S_N(bigint_tree *S, LinkedList<TreeNode> *N, double k)
    {
        auto chunks = create_chunks(N);
        auto result = new LinkedList<TreeNode>();
        bigfloat K{k};

        while (!chunks->empty()) {
            auto found_numbers = new LinkedList<bigint *> *[NUM_THREADS];
            auto iteration_chunks = new TreeNode *[NUM_THREADS];
            for(int i = 0; i < NUM_THREADS; i++) {found_numbers[i] = nullptr; iteration_chunks[i] = nullptr;}



            for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
                auto pts = chunks->top();
                chunks->pop();
                if (pts == nullptr) break;
                iteration_chunks[i] = pts;
            }

            #pragma omp parallel
            {
                int i = omp_get_thread_num();

                if (iteration_chunks[i] != nullptr) {
                    found_numbers[i] = generate_twinsmooth(iteration_chunks[i], S, K);
                    delete[] iteration_chunks[i];
                }
            }

            for (int i = 0; i < (NUM_THREADS); i++) {
                if (found_numbers[i] != nullptr) {
                    result->push(S->merge_return_inserted(found_numbers[i]));
                }
            }

            delete[] iteration_chunks;
            delete[] found_numbers;

        }
        chunks->clear();
        delete chunks;

        return result;
    }
}