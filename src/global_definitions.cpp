#include "global_definitions.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <tuple>
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

void LoadConfigFile()
{
    std::ifstream conf("config.conf");
    std::string line;

    auto lowercase = [](std::string &s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
    };

    auto divide = [](const std::string &s, char c) {
        size_t pos = s.find_first_of(c);
        return std::tuple<std::string, std::string>{
            s.substr(0, pos), s.substr(pos + 1, s.size() - pos - 1)
        };
    };

    while (std::getline(conf, line)) {
        if (line[0] == '#') // skip comment
            continue;
        lowercase(line);
        auto [name, value] = divide(line, ' ');

        if (name == "maxbitsize")
            GLOBALS.MaxBitSize = std::stoi(value);
        else if (name == "minbitsizetosave")
            GLOBALS.MinBitSizeToSave = std::stoi(value);
        else if (name == "maxbitsizetosave")
            GLOBALS.MaxBitSizeToSave = std::stoi(value);
        else if (name == "optimizationtype") {
            if (value == "no_optimization")
                GLOBALS.OptimizationType =
                    static_cast<int>(OPTIMIZATION_LEVELS::NO_OPTIMIZATION);
            else if (value == "constant_range_optimization")
                GLOBALS.OptimizationType = static_cast<int>(
                    OPTIMIZATION_LEVELS::CONSTANT_RANGE_OPTIMIZATION);
            else if (value == "variable_range_optimization")
                GLOBALS.OptimizationType = static_cast<int>(
                    OPTIMIZATION_LEVELS::VARIABLE_RANGE_OPTIMIZATION);
            else if (value == "global_k_optimization")
                GLOBALS.OptimizationType = static_cast<int>(
                    OPTIMIZATION_LEVELS::GLOBAL_K_OPTIMIZATION);
        } else if (name == "smoothness")
            GLOBALS.Smoothness = std::stoi(value);
        else if (name == "maxfilesize")
            GLOBALS.MaxFileSize = std::stoi(value);
        else if (name == "range")
            GLOBALS.RangeCurrent = std::stoi(value);
        else if (name == "k")
            GLOBALS.KCurrent = std::stod(value);
        else if (name == "outputfile")
            GLOBALS.OutputFile = value;
        else if(name == "maxnforprimecalcs")
            GLOBALS.MaxNForPrimeCalcs = std::stoi(value);
    }
}

std::vector<int> &SearcherGlobals::GetRelevantPrimes()
{
    if (RelevantPrimes.empty()) {
        RelevantPrimes.push_back(2);
        for (int i = 3; i < Smoothness + 2; i += 2) {
            for (int j = 0; j < RelevantPrimes.size() &&
                 RelevantPrimes[j] * RelevantPrimes[j] <= i;
                 j++) {
                if (i % RelevantPrimes[j] == 0)
                    goto not_a_prime;
            }
            RelevantPrimes.push_back(i);
        not_a_prime:;
        }
    }
    return RelevantPrimes;
}

std::map<int, int> Factors(mpz_class *number)
{
    auto &primes = GLOBALS.GetRelevantPrimes();

    std::map<int, int> factors{};
    mpz_class n{ *number };
    for (auto &prime : primes) {
        while (n % prime == 0) {
            if (factors.find(prime) == factors.end()) {
                factors[prime] = 0;
            }

            factors[prime]++;
            n = n / prime;
        }
    }

    return factors;
}

} // namespace globals