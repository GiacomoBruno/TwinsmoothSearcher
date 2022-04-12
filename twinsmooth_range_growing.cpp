#include "twinsmooth_range_growing.h"
#include "static_twinsmooth.h"

void s_twinsmooth_range_growing::print_top_numbers()
{
    auto iter = S->end();
    output_file.open();
    lg->logl("\tTop numbers found:");
    for(int i = 0; i < amount_of_top_twins_to_log && iter != nullptr; i++)
    {
        lg->logl("\t\t", iter->val);
        output_file.printn("%Zd\n", *iter->val);
        iter = iter->prev;
    }
    output_file.flush();
    output_file.close();
    lg->newline();
}

s_twinsmooth_range_growing::s_twinsmooth_range_growing(size_t s, size_t srange, size_t erange, size_t step, size_t top_to_save) : s_twinsmooth(s), start_range(srange), end_range(erange), step_range(step), current_range(srange), amount_of_top_twins_to_log(top_to_save) {

}

void s_twinsmooth_range_growing::start() {
    lg->logl("start algorithm [GROWING K OPTIMIZATION] on threads ", NUM_THREADS);
    lg->log("start range ", start_range);
    lg->log(" end range ", end_range);
    lg->log(" step range ", step_range);
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    init_set();

}


void s_twinsmooth_range_growing::execute() {
    using st = static_twinsmooth;
    do
    {
        benchmark b;
        b.start_bench();
        N->clear(); delete N;
        if(current_range == start_range)
        {
            //current_range = current_range - step_range;
            N = st::r_iteration_S_S(S, 1000);
            //output_file.save_list(N);
            lg->log("finished SxS iteration for range ", 1000);
            //lg->log( ", old K ", current_range - step_range);
            lg->log(" and found ", N->size()); lg->log(" in ");
        }
        else
        {
            N = st::r_iteration_S_S(S, current_range);
            //N = st::r_iteration_S_S(S, current_range, current_range - step_range);
            //output_file.save_list(N);
            lg->log("finished SxS iteration for K ", current_range);
            lg->log( ", old range ", current_range - step_range);
            lg->log(" and found ", N->size()); lg->log(" in ");
        }

        b.conclude_bench();

        b.start_bench();
        size_t found_numbers = 0;
        do{
            benchmark lb;
            lb.start_bench();
            auto tmp = st::r_iteration_S_N(S, N, current_range);
            N->clear(); delete N;
            N = tmp;
            //output_file.save_list(N);
            found_numbers += N->size();
            lg->log("\tfinished iteration SxN and found ", N->size()); lg->log(" in ");
            lb.conclude_bench();
        }while(!N->empty());
        lg->log("finished SxN iterations for K ", current_range);
        lg->log(" and found ", found_numbers);
        lg->log(" in ");
        b.conclude_bench();
        print_top_numbers();
        increment_range();
    }while(current_range < end_range);

}

void s_twinsmooth_range_growing::increment_range() {
    current_range += step_range;
}
//time in seconds : [942.326436] - CPU time in seconds: [7120.137667]
//found in total: [ 6241622  ]
void s_twinsmooth_range_growing::terminate() {
    output_file.inverse_reorder();
    output_file.close();
    N->clear(); delete N;
    lg->logl("found in total: ", S->size());
    S->cleanup();
    delete S;
}


