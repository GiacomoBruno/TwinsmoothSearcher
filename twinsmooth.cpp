#include "twinsmooth.h"
#include "static_twinsmooth.h"


void s_twinsmooth::load_files() {
    for(size_t i = 1; i < smoothness; i++)
    {
        CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(i), std::fstream::in, i);
        if(previously_found_twins.exists()) {
            std::cout << "loading file of smoothness = " << i << std::endl;
            previously_found_twins.load_file(S);
        }
    }
}

void s_twinsmooth::init_set() {
    if(S->get_size() == 0)
    {
        for(size_t i = 1; i < smoothness; i++)
        {
            auto num = bigint_new;
            bigint_init(num, i);
            S->insert(num);
        }
    }
    else
    {
        for(size_t i = 1; i <= smoothness; i++)
        {
            auto num = bigint_new;
            bigint_init(num, i);

            auto inserted = S->search_delete_source(num);
            if(inserted == nullptr) inserted = S->insert(num);
            N->push(inserted);
        }
    }
}

void s_twinsmooth::start() {
    lg->logl("start algorithm [NO OPTIMIZATIONS] on threads ", NUM_THREADS);
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    load_files();
    init_set();
}

void s_twinsmooth::terminate() {
    output_file.reorder();
    output_file.close();
    lg->logl("found in total: ", S->get_size());

    N->clear();
    delete N;
    S->cleanup();
    delete S;

}

void s_twinsmooth::execute() {
    using st = static_twinsmooth;
    //S is ready, N is empty
    do {
        if(N->empty()) {
            N->clear(); delete N;
            output_file.save_tree(S);
            N = st::iteration_S_S(S);
        }
        else {
            auto tmp = st::iteration_S_N(S, N);
            N->clear(); delete N;
            N = tmp;
        }

        output_file.save_list(N);
        lg->logl("new twinsmooth found: ", N->size());
    } while(!N->empty());
}