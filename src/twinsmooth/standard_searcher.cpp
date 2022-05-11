#include "../../include/twinsmooth/standard_searcher.h"
#include "../../include/utilities.h"
#include "../../include/twinsmooth/iterators.h"
#include "../../include/benchmark.h"

using namespace utilities;

namespace twinsmooth {

    void standard_searcher::initialize() {
        LG->log("starting algorithm [STANDARD]\n\n");

        S = new bigint_tree;

        for (unsigned long long i = 1; i < SMOOTHNESS; i++) {
            S->insert(new bigint(i));
        }
    }

    void standard_searcher::run() {
        benchmark bench;

        bench.start_bench();
        N = twinsmooth::iteration_S_S(S);
        LG->log("SxS iteration completed, found: ", N->size());
        LG->log(" - "); bench.conclude_bench();
        LG->newline();

        while(!N->empty())
        {
            bench.start_bench();
            auto new_N = iteration_S_N(S, N);
            N->clear(); delete N;
            N = new_N;




            LG->log("SxN iteration completed, found: ", N->size());
            LG->log(" - "); bench.conclude_bench();
            LG->newline();
        }
    }

    void standard_searcher::dispose() {
        LG->logl("total twinsmooths found: ", S->size());

        LG->log_top_of_tree(S, TOP_NUMBERS_AMOUNT);
        if(SAVE_ALL_TWINSMOOTHS)
            TWINSMOOTH_OUTPUT->f_log_tree(S);

        N->clear(); delete N;
        S->cleanup(); delete S;
    }
}


