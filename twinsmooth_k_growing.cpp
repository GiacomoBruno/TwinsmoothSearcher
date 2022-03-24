#include "twinsmooth_k_growing.h"
#include "twinsmooth_k.h"
#include "static_twinsmooth.h"

void s_twinsmooth_k_growing::print_top_numbers()
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

s_twinsmooth_k_growing::s_twinsmooth_k_growing(size_t s, double sk, double ek, double ks, size_t top_to_save) : s_twinsmooth(s), start_k(sk), end_k(ek), step_k(ks), amount_of_top_twins_to_log(top_to_save) {
    current_k = bigfloat_new;
    old_k = bigfloat_new;
    bigfloat_init(current_k, 2.0);
    bigfloat_init(old_k, sk);
    cur_k = 2.0;
}

void s_twinsmooth_k_growing::start() {
    lg->logl("start algorithm [GROWING K OPTIMIZATION] on threads ", NUM_THREADS);
    lg->log("start K ", start_k);
    lg->log(" end K ", end_k);
    lg->log(" step K ", step_k);
    lg->logl(" first iteration K [ 2.0 ]");
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);

    if(S->empty())
    {
        init_set();
        LinkedTree* unfilteredN;
        using st = static_twinsmooth;

        unfilteredN = st::k_iteration_S_S(S, current_k);
        N->clear(); delete N;
        N = S->merge_return_inserted(unfilteredN);
        lg->logl("finished FIRST SxS iteration and found ", N->size());
        cur_k = start_k;
        bigfloat_set(current_k, start_k);
        size_t found_numbers = 0;
        while(!N->empty())
        {
            unfilteredN = st::k_iteration_S_N(S, N, current_k);
            N->clear(); delete N;
            N = S->merge_return_inserted(unfilteredN);
            found_numbers += N->size();
            lg->log("\titeration concluded with k = ", cur_k);
            lg->logl("found ", N->size());
        }
        lg->log("finished FIRST SxN iterations for K ", cur_k);
        lg->logl(" and found ", found_numbers);
        print_top_numbers();
        increment_k();
    }
    else
    {
        lg->logl("found unfinished work, skipping first iteration");
        cur_k = start_k;
        bigfloat_set(old_k, start_k - step_k);
        bigfloat_set(current_k, start_k);
    }
}

void s_twinsmooth_k_growing::execute() {
    using st = static_twinsmooth;
    LinkedTree* unfilteredN;
    do
    {
        benchmark b;
        b.start_bench();
        unfilteredN = st::k_iteration_S_S(S, current_k, old_k);
        N->clear(); delete N;
        N = S->merge_return_inserted(unfilteredN);
        //output_file.save_list(N);
        lg->log("finished SxS iteration for K ", cur_k);
        lg->log( ", old K ", cur_k - step_k);
        lg->log(" and found ", N->size()); lg->log(" in ");
        b.conclude_bench();

        b.start_bench();
        size_t found_numbers = 0;
        do{
            benchmark lb;
            lb.start_bench();
            unfilteredN = st::k_iteration_S_N(S, N, current_k);
            N->clear(); delete N;
            N = S->merge_return_inserted(unfilteredN);
            //output_file.save_list(N);
            found_numbers += N->size();
            lg->log("\tfinished iteration SxN and found ", N->size()); lg->log(" in ");
            lb.conclude_bench();
        }while(!N->empty());
        lg->log("finished SxN iterations for K ", cur_k);
        lg->log(" and found ", found_numbers);
        lg->log(" in ");
        b.conclude_bench();
        print_top_numbers();
        increment_k();
    }while(cur_k < end_k);

    bigfloat_free(current_k);
    bigfloat_free(old_k);
}

void s_twinsmooth_k_growing::increment_k() {
    bigfloat_set(old_k, cur_k);
    cur_k += step_k;
    bigfloat_set(current_k, cur_k);
}

void s_twinsmooth_k_growing::terminate() {
    output_file.inverse_reorder();
    output_file.close();
    delete N;
    lg->logl("found in total: ", S->get_size());
    S->cleanup();
    delete S;
}


