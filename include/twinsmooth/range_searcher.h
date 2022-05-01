#pragma once

#include "searcher.h"


namespace twinsmooth {

    class range_searcher : public searcher {
    private:
        int range;
    protected:

        void initialize() override;
        void run() override;
        void dispose() override;
    public:
        explicit range_searcher(int r) : range(r) {};
    };

}
