
#include "utilities.hpp"
#include "logger.hpp"
#include "twins/searcher_menu.hpp"
#include "twins/searchers.hpp"
#include <iostream>

namespace twins
{

    int option_menu()
    {
        std::cout << "(0) no optimizations" <<
        std::endl << "(1) range optimization" <<
        std::endl << "(2) k optimization" <<
        std::endl << "(3) growing k optimization" <<
        std::endl;
        return utilities::get_int("select: ");
    }

    base_searcher* get_searcher()
    {
        int choice = option_menu();

        base_searcher* s{nullptr};
        simple_logger log{utilities::SMOOTHNESS};

        switch(choice){
            case 0: s = new twins::std_searcher(utilities::SMOOTHNESS); break;
            case 1: s = new twins::rng_searcher(utilities::SMOOTHNESS, utilities::get_int("range: ")); break;
            case 2: s = new twins::kvl_searcher(utilities::SMOOTHNESS, utilities::get_double("k: ")); break;
            default: log.print("failed to create searcher!\n"); break;
        }

        return s;
    }
}