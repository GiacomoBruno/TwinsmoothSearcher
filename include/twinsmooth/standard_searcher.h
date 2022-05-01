#pragma once
#include "searcher.h"

namespace twinsmooth {

    class standard_searcher : public searcher {
    protected:

        void initialize() override;
        void run() override;
        void dispose() override;
    };

}

