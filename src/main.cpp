#include "twins/searchers.hpp"
#include "logger.hpp"
int main(int argc, char** argv)
{

    twins::std_searcher src{10};

    src.execute();

    //auto s = twinsmooth::get_searcher();
    //s->execute();
    auto& log = twins::logger::get();

    log.print("Hello there!", 10);

    return 0;

}


