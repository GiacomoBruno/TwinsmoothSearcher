//
// Created by Giacomo Bruno on 16/03/22.
//

#include "twinsmooth_k_growing.h"
#include "file_manager.h"

void twinsmooth_k_growing::execute() {
    executor->load_files();

    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    std::cout << "executing twinsmooth calculation on threads: {" << NUM_THREADS << "}" << std::endl;
    std::cout << "mode = k growing with start: {" << kstart << "}, end: {"<< kend <<"}, kstep: {"<<kstep<<"}" << std::endl;
    std::cout << "smoothness = {" << smoothness << "}" <<std::endl;
    if(executor->get_results()->get_size() == 0)
    {
        executor->init_starting_set();
        executor->set_k(2.0);
        auto iteration_result = executor->iteration_S_S();
        auto new_iteration_results = executor->get_results()->merge_return_inserted(iteration_result);
        output.save_list(new_iteration_results);
        std::cout << "completed initial step SxS with {" << new_iteration_results->size() << "} results" << std::endl;
        executor->set_k(kstart);
        do {
            iteration_result = executor->iteration_S_N(new_iteration_results);
            new_iteration_results->clear();
            delete new_iteration_results;
            new_iteration_results = executor->get_results()->merge_return_inserted(iteration_result);
            output.save_list(new_iteration_results);
            std::cout << "completed initial step SxN with {" << new_iteration_results->size() << "} results" << std::endl;

        } while(!new_iteration_results->empty());
    }


    double k = kstart + kstep;
    double oldk = kstart;
    while(k < kend)
    {
        auto iteration_result = executor->iteration_S_S_no_small_k(oldk);
        auto new_iteration_results = executor->get_results()->merge_return_inserted(iteration_result);
        output.save_list(new_iteration_results);
        std::cout << "completed {" << k << "} step SxS with {" << new_iteration_results->size() << "} results" << std::endl;

        do {
            iteration_result = executor->iteration_S_N(new_iteration_results);
            new_iteration_results->clear();
            delete new_iteration_results;
            new_iteration_results = executor->get_results()->merge_return_inserted(iteration_result);
            output.save_list(new_iteration_results);
            std::cout << "completed {" << k << "} sub-step SxN with {" << new_iteration_results->size() << "} results" << std::endl;

        }while(!new_iteration_results->empty());

        oldk = k;
        k += kstep;
    }

    output.reorder();
}
