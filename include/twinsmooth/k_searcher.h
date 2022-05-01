#pragma once

#include "searcher.h"


namespace twinsmooth {

    class k_searcher : public searcher {
    private:
        double k;
    protected:

        void initialize() override;
        void run() override;
        void dispose() override;
    public:
        explicit k_searcher(double kval) : k(kval) {};
    };

}
