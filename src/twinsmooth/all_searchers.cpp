#include "../../include/twinsmooth/all_searchers.h"
#include "../../include/twinsmooth/standard_searcher.h"
#include "../../include/twinsmooth/range_searcher.h"
#include "../../include/twinsmooth/k_searcher.h"
#include "../../include/twinsmooth/growing_k_searcher.h"

using namespace utilities;

namespace twinsmooth {

    int option_menu()
    {
        std::cout << "(0) no optimizations" <<
        std::endl << "(1) range optimization" <<
        std::endl << "(2) k optimization" <<
        std::endl << "(3) growing k optimization" <<
        std::endl;
        return get_int("select: ");
    }

    searcher* get_searcher()
    {
        int choice = option_menu();

        twinsmooth::searcher* s{nullptr};

        switch(choice){
            case 0: s = new twinsmooth::standard_searcher(); break;
            case 1: s = new twinsmooth::range_searcher(get_int("range: ")); break;
            case 2: s = new twinsmooth::k_searcher(get_double("k: ")); break;
            case 3: s = new twinsmooth::growing_k_searcher(get_double("starting k: "), get_double("final k: "),
                                                           get_double("step k: ")); break;
            default: LG->log("failed to create searcher!\n"); break;
        }

        return s;
    }
}