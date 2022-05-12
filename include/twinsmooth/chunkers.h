//
// Created by Giacomo Bruno on 21/04/22.
//

#pragma once
#include "../bigint_tree.h"
#include "../list.h"

constexpr int CHUNK_SIZE = 200;

namespace twinsmooth {

     LinkedList<TreeNode *> *create_chunks(bigint_tree *input);

     LinkedList<TreeNode *> *create_chunks(LinkedList<TreeNode> *input);

}

