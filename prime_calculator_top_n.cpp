#include "prime_calculator_top_n.h"

void prime_calculator_top_n::start() {
    lg->logl("start algorithm [PRIME CALCULATION TOP N] on threads ", NUM_THREADS);
    lg->log("number of primes to look for: ", primes_to_find);
    lg->logl(" twinsmooth set is correctly ordered: ", already_ordered);
    lg->logl("smoothness ", smoothness);
    output_file = CappedFile(PRIMES_FN, OUT_FOLDER(smoothness), std::fstream::out, smoothness);
    output_file.open();
    if(!already_ordered)
    {
        CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::in, smoothness);
        previously_found_twins.inverse_reorder();
        previously_found_twins.close();
    }
}

void prime_calculator_top_n::execute() {

    size_t counter = 0;

    CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::in, smoothness);
    previously_found_twins.open();
    while(counter < primes_to_find) {
        bigint twin = previously_found_twins.get_bigint();

        if (twin != nullptr) {
            mpz_mul_2exp(*twin, *twin, 1);
            mpz_add_ui(*twin, *twin, 1);
            if (mpz_probab_prime_p(*twin, 1000)) {
                output_file.printn("%Zd\n", *twin);
                counter++;
            }
            bigint_free(twin);
        }
        else return;
    }
}

void prime_calculator_top_n::terminate() {
    output_file.close();
    delete N;
    delete S;

}
