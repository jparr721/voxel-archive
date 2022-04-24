#pragma once

#include "math.h"

namespace vx {
    template<typename T>
    inline auto radians(T deg) -> T {
        return deg * (kPi / 180);
    }

    template<typename T>
    inline auto degrees(T rad) -> T {
        return rad * (180 / kPi);
    }
}// namespace vx
