#include "colors.h"

namespace vx::util {
    auto u32ToRgba(u32 c) -> vec4 {
        return vec4(((c >> 24) & 0xFF), ((c >> 16) & 0xFF), ((c >> 8) & 0xFF), ((c >> 0) & 0xFF));
    }

    auto rgbaToU32(vec4 rgba) -> u32 {
        const u32 r = (u32) rgba.r & 0xFF;
        const u32 g = (u32) rgba.g & 0xFF;
        const u32 b = (u32) rgba.b & 0xFF;
        const u32 a = (u32) rgba.a & 0xFF;
        return (r << 24) | ((g & 255) << 16) | ((b & 255) << 8) | (a & 255);
    }
}// namespace vx::util
