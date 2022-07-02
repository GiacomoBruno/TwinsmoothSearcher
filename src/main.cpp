#include "twins/searchers.hpp"
#include "logger.hpp"
#include "utilities.hpp"
#include "twins/searcher_menu.hpp"

int main(int argc, char** argv)
{

    utilities::init_machine();

    auto s = twins::get_searcher();
    s->execute();

    return 0;

}


