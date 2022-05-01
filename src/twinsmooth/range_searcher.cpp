#include "../../include/twinsmooth/range_searcher.h"

#include "../../include/twinsmooth/iterators.h"
#include "../../include/utilities.h"
#include "../../include/benchmark.h"

using namespace utilities;

namespace twinsmooth {

    void range_searcher::initialize() {

        LG->log("starting algorithm [RANGE]\n");
        LG->logl("range : ", range);
        LG->newline();

        S = new bigint_tree;

        for (int i = 1; i < SMOOTHNESS; i++) {
            S->insert(new bigint(i));
        }
    }

    void range_searcher::run() {

        benchmark bench;
        bench.start_bench();
        N = twinsmooth::iteration_S_S(S, range);
        LG->log("SxS iteration completed, found: ", N->size());
        LG->log(" - "); bench.conclude_bench();
        LG->newline();

        while(!N->empty())
        {
            bench.start_bench();
            auto new_N = iteration_S_N(S, N, range);
            N->clear(); delete N;
            N = new_N;

            LG->log("SxN iteration completed, found: ", N->size());
            LG->log(" - "); bench.conclude_bench();
            LG->newline();
        }
    }

    void range_searcher::dispose() {

        LG->log_top_of_tree(S, TOP_NUMBERS_AMOUNT);

        N->clear(); delete N;
        S->cleanup(); delete S;
    }

}