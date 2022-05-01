#include "../../include/twinsmooth/growing_k_searcher.h"

#include "../../include/twinsmooth/iterators.h"
#include "../../include/utilities.h"
#include "../../include/benchmark.h"

using namespace utilities;

namespace twinsmooth {

    void growing_k_searcher::initialize() {
        LG->log("starting algorithm [GROWING K]\n");
        LG->logl("starting k: ", starting_k);
        LG->logl("final k:    ", final_k);
        LG->logl("step k:     ", step_k);
        LG->newline();
        S = new bigint_tree;

        for (unsigned long long i = 1; i < SMOOTHNESS; i++) {
            S->insert(new bigint(i));
        }
    }

    void growing_k_searcher::run() {
        benchmark bench;

        bench.start_bench();
        N = iteration_S_S(S);
        LG->logl("\ncurrent k = ", k);
        LG->log("SxS iteration completed, found: ", N->size());
        LG->log(" - "); bench.conclude_bench();
        LG->newline();


        while(!N->empty())
        {
            bench.start_bench();
            auto tmp = iteration_S_N(S, N, k);
            N->clear(); delete N;
            N = tmp;

            LG->log("SxN iteration completed, found: ", N->size());
            LG->log(" - "); bench.conclude_bench();
            LG->newline();

        }
        k+= step_k;
        while(k <= final_k){
            LG->logl("\ncurrent k = ", k);
            N->clear(); delete N;

            bench.start_bench();

            N = iteration_S_S(S, k);

            LG->log("SxS iteration completed, found: ", N->size());
            LG->log(" - "); bench.conclude_bench();
            LG->newline();

            while(!N->empty())
            {
                bench.start_bench();

                auto tmp = iteration_S_N(S,N,k);
                N->clear(); delete N;
                N = tmp;

                LG->log("SxN iteration completed, found: ", N->size());
                LG->log(" - "); bench.conclude_bench();
                LG->newline();
            }

            LG->logl("numbers found with k: ", k);
            LG->log_top_of_tree(S, TOP_NUMBERS_AMOUNT);

            k += step_k;

        }

    }

    void growing_k_searcher::dispose() {
        N->clear(); delete N;
        S->cleanup(); delete S;
    }

}