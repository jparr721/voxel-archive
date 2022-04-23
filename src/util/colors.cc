#include "colors.h"

namespace vx::util {
    auto u32ToRgba(u32 c) -> vec4 {
        return glm::vec4(((c >> 24) & 0xFF) / 255.0f, ((c >> 16) & 0xFF) / 255.0f, ((c >> 8) & 0xFF) / 255.0f,
                         ((c >> 0) & 0xFF) / 255.0f);
    }
}// namespace vx::util
