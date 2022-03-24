//
// Created by Giacomo Bruno on 24/03/22.
//

#include "prime_calculator.h"
#include "static_twinsmooth.h"
void prime_calculator::execute() {
    delete N;
    N = static_twinsmooth::prime_iteration(S);
}

void prime_calculator::start() {
    lg->logl("start algorithm [PRIME CALCULATION] on threads ", NUM_THREADS);
    lg->logl("smoothness ", smoothness);
    load_files();
}

void prime_calculator::terminate() {
    output_file = CappedFile(PRIMES_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);

    output_file.save_list(N);

    output_file.reorder();
    output_file.close();
    lg->logl("found in total: ", N->size());
    S->cleanup();
    N->clear();
    delete N;
    delete S;


}

void prime_calculator::load_files() {
    CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::in, smoothness);
    if(previously_found_twins.exists()) {
        lg->logl("loading file of smoothness = ",smoothness);
        previously_found_twins.load_file(S);
    }
}
