#include "twinsmooth_k.h"
#include "static_twinsmooth.h"


s_twinsmooth_k::s_twinsmooth_k(size_t s, double v) : s_twinsmooth(s), k_value(v) {
    k = bigfloat_new;
    bigfloat_init(k,v);
}

void s_twinsmooth_k::execute() {
    using st = static_twinsmooth;
    //S is ready, N is empty
    do {
        if(N->empty()) {
            N->clear(); delete N;
            output_file.save_tree(S);
            N = st::k_iteration_S_S(S, k);
        }
        else {
            auto tmp = st::k_iteration_S_N(S, N, k);
            N->clear(); delete N;
            N = tmp;
        }

        output_file.save_list(N);
        lg->logl("new twinsmooth found: ", N->size());
    }while(!N->empty());

    bigfloat_free(k);
}

void s_twinsmooth_k::start() {
    lg->logl("start algorithm [CONSTANT K OPTIMIZATION] on threads ", NUM_THREADS);
    lg->logl("K ", k_value);
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    load_files();
    init_set();
}

