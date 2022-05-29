#include "timer.h"

namespace vx::util {
    void Timer::start() { currentTime_ = clock::now(); }

    auto Timer::elapsed() -> double const {
        return std::chrono::duration_cast<durationSeconds>(clock::now() - currentTime_).count();
    }
}// namespace vx::util
