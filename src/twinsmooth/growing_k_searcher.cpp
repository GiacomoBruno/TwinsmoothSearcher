#include "../../include/twinsmooth/growing_k_searcher.h"

#include "../../include/twinsmooth/iterators.h"
#include "../../include/utilities.h"
#include "../../include/benchmark.h"
#include <map>
#include <iostream>
using namespace utilities;



namespace twinsmooth {
    struct twins_stats{
        unsigned long long min;
        unsigned long long max;
        unsigned long long avg;
        unsigned long long med;
        unsigned long long avg2;
        unsigned long long med2;
    };

    std::ostream& operator<<(std::ostream& stream, const twins_stats& stats)
{
    stream << "MIN[" << std::setw(5) << stats.min << "]\n"
    << "MAX[" << std::setw(5) << stats.max <<"]\n"
    << "AVG[" << std::setw(5) << stats.avg << "]\n"
    << "MED[" << std::setw(5) << stats.med << "]\n"
    << "AV2[" << std::setw(5) << stats.avg2 << "]\n"
    << "ME2[" << std::setw(5) << stats.med2 << "]\n";

    return stream;

}


    twins_stats max_twins_found(bigint_tree* S)
    {
        std::map<int, int> stats;
        auto iter = S->begin();

        unsigned long long max = 0;
        unsigned long long min = 5000;

        while(iter != nullptr)
        {
            if(stats.contains(iter->twins_found))
                stats[iter->twins_found] += 1;
            else
                stats[iter->twins_found] = 1;

            if(iter->twins_found > max) max = iter->twins_found;
            if(iter->twins_found < min) min = iter->twins_found;
            iter = iter->next;
        }

        unsigned long long average = 0;
        unsigned long long tot = 0;


        for(auto&& a : stats)
        {
            tot += a.second;
            average = (a.first * a.second) + average;
            //std::cout << "N = " << a.first << " >> " << a.second <<std::endl;
        }
        unsigned long long median_point = tot / 2;
        unsigned long long counter = 0;
        unsigned long long med = 0;

        unsigned long long second_average = 0;
        unsigned long long second_tot = 0;
        unsigned long long second_med = 0;

        unsigned long long quarter = tot / 2 + median_point / 2;
        average /= tot;
        for(auto&& a : stats)
        {
            counter += a.second;
            if(counter >= median_point && med == 0) {
                med = a.first;
            }

            if(counter >= quarter && second_med == 0)
            {
                second_med = a.first;
            }

            if(a.first > average)
            {
                second_tot += a.second;
                second_average = (a.first * a.second) + second_average;
            }
        }


        second_average /= second_tot;

        return twins_stats{min, max, average, med, second_average, second_med};
    }

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

            //N = iteration_S_S(S, k);
            auto stats = max_twins_found(S);
            std::cout << stats << std::endl;
            auto avg = stats.avg2;
            //LG->log("stats: \n");
            //LG->logl("MIN = ", stats.min);
            //LG->logl("MAX = ", stats.max);
            //LG->logl("AVG = ", stats.avg);
            //LG->logl("MED = ", stats.med);

            N = S->copy([=] (TreeNode tn) {
                return tn->twins_found > (avg);
            });

            LG->log("Twins that generated more than ");
            LG->log("", avg);
            LG->log(" other twins: ", N->size());
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
        LG->logl("total twinsmooths found: ", S->size());

        if(SAVE_ALL_TWINSMOOTHS)
            TWINSMOOTH_OUTPUT->f_log_tree(S);
        N->clear(); delete N;
        S->cleanup(); delete S;
    }

}