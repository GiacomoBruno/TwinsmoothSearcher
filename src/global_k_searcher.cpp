#include "searcher.hpp"

#define GET_MAX_BY_K() \
    do { \
        if (!chunk.empty()) { \
            { \
                mpf_class f{ *chunk[0] }; \
                f *= GLOBALS.KCurrent; \
                max_y = f; \
            } \
            { \
                mpf_class f{ *chunk[0] }; \
                f *= (2 - GLOBALS.KCurrent); \
                max_z = f; \
            } \
        } else \
            return; \
    } while (false)

namespace searcher {

template <>
void generate_twins<globals::OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION>(
    CHUNKARR &chunk,
    PSET &S,
    PSET &output)
{
    mpz_class d, delta, m1, result;

    mpz_class max_y;
    mpz_class max_z;
    GET_MAX_BY_K();

    int max = last_array_element(chunk);

    for (int i = 0; i < max; i++) {
        auto &x = chunk[i];
        auto y = S.upper_bound(x);
        auto z = S.lower_bound(x);
        // forward
        while (y != S.end() && **y < max_y) {
            FORWARD_CALCS(x, y);

            if (d == 0) {
                ADD_TO_SET();
            }
            std::advance(y, 1);
        }
        // backward
        if (mpz_cmp((*z)->get_mpz_t(), x->get_mpz_t()) == 0)
            continue;

        while (z != S.begin() && **z < max_z) {
            BACKWARD_CALCS(x, y);

            if (d == 0) {
                ADD_TO_SET();
            }
            std::advance(z, -1);
        }
    }
}

template <>
void iteration<globals::OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION>(
    PVEC &io,
    PSET &S)
{
    static bool need_swap = true;
    static double k_tmp = GLOBALS.KCurrent;

    if (S.size() < 100000) {
        GLOBALS.KCurrent = 2.0;
    } else if (need_swap) {
        need_swap = false;
        GLOBALS.KCurrent = k_tmp;
    }

    CHUNKVEC chunks{};
    generate_chunks(io, chunks);

    std::vector<PSET> temp_results(chunks.size());

    GLOBALS.ThreadPool.push_loop(0, chunks.size(), [&](int a, int b) {
        for (int i = a; i < b; ++i) {
            generate_twins<globals::OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION>(
                chunks[i], S, temp_results[i]);
        }
    });
    GLOBALS.ThreadPool.wait_for_tasks();

    io.clear();

    // parallelized check for collisions in the new results
    while (temp_results.size() > 1) {
        std::vector<PSET> IN{ temp_results };
        std::vector<PSET> OUT;
        check_result_collisions(IN, OUT);
        temp_results = OUT;
    }

    for (auto *n : temp_results[0]) {
        S.insert(n);
        io.push_back(n);
    }
}

} // namespace searcher