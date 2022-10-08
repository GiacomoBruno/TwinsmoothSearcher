#include "global_definitions.hpp"

namespace globals
{

    std::string PrintOptimizationLevel(OPTIMIZATION_LEVELS level)
    {
        using O = OPTIMIZATION_LEVELS;
        switch(level)
        {
            case O::NO_OPTIMIZATION:
                return "No optimizations";
            case O::CONSTANT_RANGE_OPTIMIZATION:
                return std::string("Constant range: ") + std::to_string(GLOBALS.RangeCurrent);
            case O::GLOBAL_K_OPTIMIZATION:
                return std::string("Global K: ") + std::to_string(GLOBALS.KCurrent);
            case O::VARIABLE_RANGE_OPTIMIZATION:
                return "Variable range";
        }
        return "";
    }
}