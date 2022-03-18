#include "twinsmooth_k_growing.h"
#include "twinsmooth_k.h"
#include "file_manager.h"

twinsmooth_k_growing::twinsmooth_k_growing(uint64_t s, double startk, double endk, double stepk)
    : twinsmooth(s), start_k(startk), end_k(endk), step_k(stepk), cur_k(2.0)
{
    current_k = bigfloat_new;
    bigfloat_init(current_k, cur_k);
    old_k = bigfloat_new;
    bigfloat_init(old_k, start_k);
}
void twinsmooth_k_growing::start() {
    //do not load from files for this implementation
    init_starting_set();
}

void twinsmooth_k_growing::set_k(double n) {
    cur_k = n;
    bigfloat_free(old_k);
    old_k = current_k;
    current_k = bigfloat_new;
    bigfloat_init(current_k, n);
}

void twinsmooth_k_growing::execute() {
    lg->newline();
    lg->logl("executing twinsmooth calculation on threads: ", NUM_THREADS);
    lg->log("mode = growing k optimization with start k = ", start_k);
    lg->log(" - max k = ", end_k);
    lg->logl(" - step = ", step_k);
    lg->logl("smoothness = ", smoothness);


    using k = twinsmooth_k;
    size_t result_counter = 0;
    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    auto b = benchmark();

    output.save_tree(results);
    //FIRST ITERATION SxS with K = 2.0
    auto i_results = k::iteration_S_S(results, current_k);
    auto new_i_results = results->merge_return_inserted(i_results);
    result_counter += new_i_results->size();
    output.save_list(new_i_results);
    //N+1 ITERATIONS SxN with K = start
    set_k(start_k);
    lg->logl("STARTING - with k = ", cur_k);
    b.start_bench();
    do{
        i_results = k::iteration_S_N(results, new_i_results, current_k);
        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);
        result_counter += new_i_results->size();
        output.save_list(new_i_results);
    } while(!new_i_results->empty());

    lg->log("FINISHED - results found: ", result_counter);
    lg->log(" "); b.conclude_bench();


    while(cur_k <= end_k) {
        set_k(cur_k + step_k);
        result_counter = 0;
        lg->logl("STARTING - with k = ",cur_k);
        b.start_bench();
        i_results = k::iteration_S_S_no_oldk(results, current_k, old_k);
        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);
        output.save_list(new_i_results);
        lg->newline();

        do {
            auto inner_bench = benchmark();
            inner_bench.start_bench();
            i_results = k::iteration_S_N(results, new_i_results, current_k);
            new_i_results->clear();
            delete new_i_results;
            new_i_results = results->merge_return_inserted(i_results);
            result_counter += new_i_results->size();
            
            lg->log("\t iteration completed, found: ", new_i_results->size());
            lg->log(" "); inner_bench.conclude_bench();
            output.save_list(new_i_results);
        } while (!new_i_results->empty());
        lg->newline();
        lg->log("FINISHED - results found: ",result_counter);
        lg->log(" "); b.conclude_bench();

    }
    output.reorder();
}

void twinsmooth_k_growing::terminate() {
    lg->logl("total twinsmooth found: ", results->get_size());
    results->cleanup();
    delete results;
}
