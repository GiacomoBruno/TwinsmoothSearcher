#pragma once
#include "../bigint.h"
#include "../bigint_tree.h"
#include "../list.h"

namespace twinsmooth {

    class searcher {
    protected:
        LinkedList<TreeNode>*       N{nullptr};
        bigint_tree*                S{nullptr};

        virtual void initialize()   = 0;
        virtual void run()          = 0;
        virtual void dispose()      = 0;

    public:
        void execute();
    };
}

