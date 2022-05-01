//
// Created by Giacomo Bruno on 21/04/22.
//

#pragma once
#include "../bigint.h"
#include "../bigfloat.h"
#include "../bigint_tree.h"


namespace twinsmooth {
    LinkedList<bigint*>* generate_twinsmooth(TreeNode* chunk, bigint_tree* S);

    LinkedList<bigint*>* generate_twinsmooth(TreeNode* chunk, bigint_tree* S, int range);

    LinkedList<bigint*>* generate_twinsmooth(TreeNode* chunk, bigint_tree* S, const bigfloat& k);
}

