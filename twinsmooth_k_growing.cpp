#include "twinsmooth_k_growing.h"
#include "twinsmooth_k.h"
#include "file_manager.h"

void print_top_numbers(LinkedTree* tree)
{
    auto iter = tree->end();
    lg->logl("\tTop numbers found:");
    for(int i = 0; i < 5 && iter != nullptr; i++)
    {
        lg->logl("\t\t", iter->val);
        iter = iter->prev;
    }
    lg->newline();
}

twinsmooth_k_growing::twinsmooth_k_growing(size_t s, double startk, double endk, double stepk)
    : twinsmooth(s), start_k(startk), end_k(endk), step_k(stepk), cur_k(2.0)
{
    current_k = bigfloat_new;
    bigfloat_init(current_k, cur_k);
    old_k = bigfloat_new;
    bigfloat_init(old_k, start_k);
}



void twinsmooth_k_growing::init_starting_set() {
    for(size_t i = 1; i <= smoothness; i++)
    {
        auto num = bigint_new;
        bigint_init(num, i);
        results->insert(num);
    }
}

void twinsmooth_k_growing::start() {
    //do not load from files for this implementation
    init_starting_set();
    delete computation_numbers;
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
    computation_numbers = results->merge_return_inserted(i_results);
    result_counter += computation_numbers->size();
    output.save_list(computation_numbers);
    //N+1 ITERATIONS SxN with K = start
    set_k(start_k);
    lg->logl("STARTING - with k = ", cur_k);
    b.start_bench();
    do{
        i_results = k::iteration_S_N(results, computation_numbers, current_k);
        computation_numbers->clear();
        delete computation_numbers;
        computation_numbers = results->merge_return_inserted(i_results);
        result_counter += computation_numbers->size();
        output.save_list(computation_numbers);
    } while(!computation_numbers->empty());

    lg->log("FINISHED - results found: ", result_counter);
    lg->log(" "); b.conclude_bench();
    print_top_numbers(results);


    while(cur_k <= end_k) {
        set_k(cur_k + step_k);
        result_counter = 0;
        lg->logl("STARTING - with k = ",cur_k);
        b.start_bench();
        i_results = k::iteration_S_S_no_oldk(results, current_k, old_k);
        computation_numbers->clear();
        delete computation_numbers;
        computation_numbers = results->merge_return_inserted(i_results);
        output.save_list(computation_numbers);
        lg->newline();

        do {
            auto inner_bench = benchmark();
            inner_bench.start_bench();
            i_results = k::iteration_S_N(results, computation_numbers, current_k);
            computation_numbers->clear();
            delete computation_numbers;
            computation_numbers = results->merge_return_inserted(i_results);
            result_counter += computation_numbers->size();
            
            lg->log("\t iteration completed, found: ", computation_numbers->size());
            lg->log(" "); inner_bench.conclude_bench();
            output.save_list(computation_numbers);
        } while (!computation_numbers->empty());
        lg->newline();
        lg->log("FINISHED - results found: ",result_counter);
        lg->log(" "); b.conclude_bench();
        print_top_numbers(results);

    }
    output.reorder();
}

void twinsmooth_k_growing::terminate() {
    lg->logl("total twinsmooth found: ", results->get_size());
    results->cleanup();
    computation_numbers->clear();
    delete computation_numbers;
    bigfloat_free(current_k);
    bigfloat_free(old_k);
    delete results;
}
