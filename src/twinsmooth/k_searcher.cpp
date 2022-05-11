#include "../../include/twinsmooth/k_searcher.h"

#include "../../include/twinsmooth/iterators.h"
#include "../../include/utilities.h"
#include "../../include/benchmark.h"

using namespace utilities;

namespace twinsmooth {

    void k_searcher::initialize() {

        LG->log("starting algorithm [K]\n");
        LG->logl("k: ", k);
        LG->newline();

        S = new bigint_tree;

        for (unsigned long long i = 1; i < SMOOTHNESS; i++) {
            S->insert(new bigint(i));
        }
    }

    void k_searcher::run() {
        benchmark bench;

        bench.start_bench();

        N = twinsmooth::iteration_S_S(S, k);
        LG->log("SxS iteration completed, found: ", N->size());
        LG->log(" - "); bench.conclude_bench();
        LG->newline();

        while(!N->empty()) {
            auto new_N = iteration_S_N(S, N, k);
            N->clear();
            delete N;
            N = new_N;

            LG->log("SxN iteration completed, found: ", N->size());
            LG->log(" - ");
            bench.conclude_bench();
            LG->newline();
        }
    }

    void k_searcher::dispose() {
        LG->logl("total twinsmooths found: ", S->size());
        LG->log_top_of_tree(S, TOP_NUMBERS_AMOUNT);
        if(SAVE_ALL_TWINSMOOTHS)
            TWINSMOOTH_OUTPUT->f_log_tree(S);
        N->clear(); delete N;
        S->cleanup(); delete S;
    }

}