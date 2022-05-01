#pragma once
#include "searcher.h"

namespace twinsmooth {

    class growing_k_searcher : public searcher {
    private:
        double k;
        double starting_k;
        double final_k;
        double step_k;
    protected:
        void initialize() override;
        void run() override;
        void dispose() override;
    public:
        growing_k_searcher(double k_start, double k_end, double k_step)
            : k(k_start), starting_k(k_start), final_k(k_end), step_k(k_step) {}
    };
}

