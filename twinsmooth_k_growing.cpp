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
    std::cout << "STARTING - with k = " << cur_k << std::endl;
    b.start_bench();
    do{
        i_results = k::iteration_S_N(results, new_i_results, current_k);
        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);
        result_counter += new_i_results->size();
        output.save_list(new_i_results);
    } while(!new_i_results->empty());

    std::cout << "FINISHED - " << result_counter << " with k = " << cur_k << " in ";
    b.conclude_bench();


    while(cur_k <= end_k) {
        set_k(cur_k + step_k);
        result_counter = 0;
        std::cout << "STARTING - with k = " << cur_k << std::endl;
        b.start_bench();
        i_results = k::iteration_S_S_no_oldk(results, current_k, old_k);
        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);
        output.save_list(new_i_results);
        do {
            auto iter_bench = benchmark();
            iter_bench.start_bench();
            i_results = k::iteration_S_N(results, new_i_results, current_k);
            
            new_i_results->clear();
            delete new_i_results;
            new_i_results = results->merge_return_inserted(i_results);
            std::cout << "\t\tcompleted iteration for " << cur_k << " found: " << new_i_results->size() << " in ";
            iter_bench.conclude_bench();
            result_counter += new_i_results->size();
            output.save_list(new_i_results);
        } while (!new_i_results->empty());
        std::cout << "FINISHED - Found " << result_counter << " new twinsmooths with k = " << cur_k << " in ";
        b.conclude_bench();

    }
    output.reorder();
}

void twinsmooth_k_growing::terminate() {
    std::cout << "found in total: " << results->get_size() << std::endl;
    results->cleanup();
    delete results;
}
