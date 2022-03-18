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
    std::filesystem::create_directories(OUT_FOLDER(smoothness));
    init_starting_set();
}

void twinsmooth_k_growing::set_k(double n) {
    cur_k = n;
    bigfloat_free(old_k);
    old_k = current_k;
    current_k = bigfloat_new;
    bigfloat_init(current_k, n);
}

void print_biggest_results(LinkedTree* tree)
{
    auto iter = tree->end();
    log->logl("\n\tbiggest results till now: ");
    for(int i = 0; i < 5 && iter != nullptr; i++, iter=iter->prev)
    {
        log->logl("\t\t", iter->val);
    }
    std::cout << std::endl;
}

void twinsmooth_k_growing::execute() {
    log->logl("executing twinsmooth calculation on threads: ", NUM_THREADS);
    log->log("mode = growing k optimization with start k = ", start_k);
    log->log(" - endk = ", end_k);
    log->logl(" - step = ", step_k);
    log->logl("smoothness = ", smoothness);

    using k = twinsmooth_k;
    size_t result_counter = 0;
    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);


    auto outer_bench = benchmark();
    output.save_tree(results);

    //FIRST ITERATION SxS with K = 2.0
    auto i_results = k::iteration_S_S(results, current_k);
    auto new_i_results = results->merge_return_inserted(i_results);
    result_counter += new_i_results->size();
    output.save_list(new_i_results);
    //N+1 ITERATIONS SxN with K = start
    set_k(start_k);

    log->logl("STARTING - with k = ", cur_k);
    outer_bench.start_bench();
    do{
        i_results = k::iteration_S_N(results, new_i_results, current_k);
        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);
        result_counter += new_i_results->size();
        output.save_list(new_i_results);
    } while(!new_i_results->empty());

    log->log("FINISHED - ", result_counter);
    log->log(" - with k = ", cur_k);
    log->log(" "); outer_bench.conclude_bench();
    print_biggest_results(results);

    while(cur_k <= end_k) {
        //increase k by step
        set_k(cur_k + step_k);
        //start counting results found
        result_counter = 0;

        log->logl("STARTING - with k = ", cur_k);
        outer_bench.start_bench();

        //do a first iteration on SxS skipping the combination for last k value
        i_results = k::iteration_S_S_no_oldk(results, current_k, old_k);


        new_i_results->clear();
        delete new_i_results;
        new_i_results = results->merge_return_inserted(i_results);

        log->log("FINISHED - SxS iteration, found: ", new_i_results->size());
        log->log(" "); outer_bench.conclude_bench();

        output.save_list(new_i_results);
        
        log->newline();
        do {
            auto iter_bench = benchmark();
            iter_bench.start_bench();
            
            //do iteration SxN
            i_results = k::iteration_S_N(results, new_i_results, current_k);
            new_i_results->clear();
            delete new_i_results;
            new_i_results = results->merge_return_inserted(i_results);
            result_counter += new_i_results->size();

            log->log("\tcompleted iteration for k = ", cur_k);
            log->log(" - found: ",  new_i_results->size());
            log->log(" "); iter_bench.conclude_bench();
            
            output.save_list(new_i_results);
        } while (!new_i_results->empty());

        log->logl("\nFINISHED - SxN iterations, found: ", result_counter);
        print_biggest_results(results);
    }
    output.reorder();
}

void twinsmooth_k_growing::terminate() {
    log->logl("total twinsmooth found: ", results->get_size());
    results->cleanup();
    delete results;
}
