#pragma once

#include <chrono>

class benchmark {
   private:
    using t = std::chrono::steady_clock::time_point;

    t start_time{};
    t end_time{};

   public:
    void start_bench();

    void conclude_bench();

    [[nodiscard]] double seconds_passed() const;
};
