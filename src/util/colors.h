#pragma once

#include "../math.h"

namespace vx::util {
    auto u32ToRgba(u32 c) -> vec4;
    auto rgbaToU32(vec4 rgba) -> u32;
}// namespace vx::util
