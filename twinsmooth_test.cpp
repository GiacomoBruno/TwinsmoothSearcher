//
// Created by Giacomo Bruno on 27/03/22.
//

#include "twinsmooth_test.h"
#include "static_twinsmooth.h"

void twinsmooth_test::execute() {
    using st = static_twinsmooth;
    //S is ready, N is
    auto f1 = bigfloat_new;
    auto f2 = bigfloat_new;

    bigfloat_init(f1, 1.1);
    bigfloat_init(f2, 1.2);

    do {
        auto tmp = st::k_iteration_S_S(S,f2,f1);
        N->clear(); delete N;
        N = tmp;

        output_file.save_list(N);
        lg->logl("new twinsmooth found: ", N->size());
    } while(!N->empty());
}
