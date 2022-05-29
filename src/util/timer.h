#pragma once

#include <chrono>
#include <spdlog/spdlog.h>

namespace vx::util {
    class Timer {
        using clock = std::chrono::high_resolution_clock;
        using durationSeconds = std::chrono::duration<double, std::ratio<1>>;

    public:
        void start();
        auto elapsed() -> double const;

    private:
        std::chrono::time_point<clock> currentTime_;
    };
}// namespace vx::util
