#pragma once

#include "math.h"

namespace vx {
    template<typename T>
    inline auto radians(T deg) -> T {
        return deg * (M_PI / 180);
    }

    template<typename T>
    inline auto degrees(T rad) -> T {
        return rad * (180 / M_PI);
    }
}// namespace vx
