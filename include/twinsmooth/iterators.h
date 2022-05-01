//
// Created by Giacomo Bruno on 21/04/22.
//

#pragma once
#include "../bigint_tree.h"

namespace twinsmooth {
    LinkedList<TreeNode>* iteration_S_S(bigint_tree *S);
    LinkedList<TreeNode>* iteration_S_N(bigint_tree* S, LinkedList<TreeNode> *N);

    LinkedList<TreeNode>* iteration_S_S(bigint_tree *S, int range);
    LinkedList<TreeNode>* iteration_S_N(bigint_tree *S, LinkedList<TreeNode> *N, int range);

    LinkedList<TreeNode>* iteration_S_S(bigint_tree *S, double k);
    LinkedList<TreeNode>* iteration_S_N(bigint_tree *S, LinkedList<TreeNode> *N, double k);
}


