#include "searcher.hpp"

namespace searcher {

template <>
void generate_twins<globals::OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION>(
    CHUNKARR &chunk,
    PSET &S,
    PSET &output)
{
    mpz_class d, delta, m1, result;
    int r = 0;

    int max = last_array_element(chunk);

    for (int i = 0; i < max; i++) {
        auto &x = chunk[i];
        // forward
        auto y = S.upper_bound(x);
        auto z = S.lower_bound(x);

        while (y != S.end() && r < GLOBALS.RangeCurrent) {
            FORWARD_CALCS(x, y);

            if (d == 0) {
                ADD_TO_SET();
            }
            r++;
            std::advance(y, 1);
        }
        // backward
        r = 0;
        if (mpz_cmp((*z)->get_mpz_t(), x->get_mpz_t()) == 0)
            continue;
        if (z != S.begin()) {
            while (r < GLOBALS.RangeCurrent) {
                BACKWARD_CALCS(x, z);
                if (d == 0) {
                    ADD_TO_SET();
                }
                r++;
                std::advance(z, -1);
            }
        }
    }
}

} // namespace searcher