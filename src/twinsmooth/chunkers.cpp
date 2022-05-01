//
// Created by Giacomo Bruno on 21/04/22.
//

#include "../../include/twinsmooth/chunkers.h"
namespace twinsmooth {

    LinkedList<TreeNode *> *create_chunks(bigint_tree *input) {
        auto result = new LinkedList<TreeNode *>();
        auto single_chunk = new TreeNode[CHUNK_SIZE];

        size_t counter = 0;
        auto iter = input->begin();
        while (iter != nullptr) {
            if (counter >= CHUNK_SIZE) {
                counter = 0;
                result->push(single_chunk);
                single_chunk = new TreeNode[CHUNK_SIZE];
            }

            single_chunk[counter] = iter;
            iter = iter->next;
            counter++;
        }

        if (counter != 0) {
            for (size_t i = counter; i < CHUNK_SIZE; i++) {
                single_chunk[i] = nullptr;
            }
            result->push(single_chunk);
        }

        return result;
    }

    LinkedList<TreeNode *> *create_chunks(LinkedList<TreeNode> *input) {
        auto result = new LinkedList<TreeNode *>();
        auto single_chunk = new TreeNode[CHUNK_SIZE];

        size_t counter = 0;
        while (!input->empty()) {
            if (counter >= CHUNK_SIZE) {
                counter = 0;
                result->push(single_chunk);
                single_chunk = new TreeNode[CHUNK_SIZE];
            }

            single_chunk[counter] = input->top();
            input->pop();
            counter++;
        }

        if (counter != 0) {
            for (size_t i = counter; i < CHUNK_SIZE; i++) {
                single_chunk[i] = nullptr;
            }
            result->push(single_chunk);
        }

        return result;

    }

}