#include "global_definitions.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace globals {

std::string PrintOptimizationLevel(OPTIMIZATION_LEVELS level)
{
    using O = OPTIMIZATION_LEVELS;
    switch (level) {
    case O::NO_OPTIMIZATION:
        return "No optimizations";
    case O::CONSTANT_RANGE_OPTIMIZATION:
        return std::string("Constant range: ") +
            std::to_string(GLOBALS.RangeCurrent);
    case O::GLOBAL_K_OPTIMIZATION:
        return std::string("Global K: ") + std::to_string(GLOBALS.KCurrent);
    case O::VARIABLE_RANGE_OPTIMIZATION:
        return "Variable range";
    }
    return "";
}

std::string format_time_point(const time_point &time, const std::string &format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    // std::tm tm = *std::gmtime(&tt); //GMT (UTC)
    std::tm tm = *std::localtime(&tt); // Locale time-zone, usually UTC by
                                       // default.
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

void iteration_header_string(size_t work_set_size)
{
    std::cout << "\nstart work on " << work_set_size << " twins at: "
              << format_time_point(
                     std::chrono::system_clock::now(), "%Y/%m/%d %H:%M:%S")
              << "\n";
}

void iteration_tail_string(
    size_t work_set_size,
    size_t last_set_size,
    double iteration_duration)
{
    std::cout << "completed iteration in: " << iteration_duration
              << " seconds and found " << work_set_size << " twins\n";

    auto expected_iteration_time = static_cast<size_t>(
        (iteration_duration / static_cast<double>(last_set_size)) *
        static_cast<double>(work_set_size));

    std::cout << "\nnext iteration expected to take at least "
              << expected_iteration_time << " seconds";
    std::cout << "\nestimate completion time: "
              << format_time_point(
                     std::chrono::system_clock::now() +
                         std::chrono::seconds(
                             static_cast<size_t>(iteration_duration)),
                     "%Y/%m/%d %H:%M:%S")
              << std::endl;
}

} // namespace globals