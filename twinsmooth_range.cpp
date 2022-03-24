#include "twinsmooth_range.h"
#include "static_twinsmooth.h"



void s_twinsmooth_range::execute() {
    using st = static_twinsmooth;
    //S is ready, N is empty
    do {
        LinkedTree* unfilteredN;
        if(N->empty()) {
            output_file.save_tree(S);
            unfilteredN = st::r_iteration_S_S(S, range);
        }
        else unfilteredN = st::r_iteration_S_N(N, range);

        N->clear(); delete N;
        N = S->merge_return_inserted(unfilteredN);
        output_file.save_list(N);
        lg->logl("new twinsmooth found: ", N->size());
    }while(!N->empty());
}

void s_twinsmooth_range::start() {
    lg->logl("start algorithm [CONSTANT RANGE OPTIMIZATION] on threads ", NUM_THREADS);
    lg->logl("range ", range);
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    load_files();
    init_set();
}