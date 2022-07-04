#include "twins/searchers.hpp"
#include "logger.hpp"
#include "utilities.hpp"
#include "twins/searcher_menu.hpp"

int main(int argc, char** argv)
{
    std::cout << sizeof(twins::avl_node) << std::endl;

    
    utilities::init_machine();

    auto s = twins::get_searcher();
    s->execute();

    return 0;

}


