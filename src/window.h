#pragma once

#include "gfx/bgfx.h"
#include "gfx/glfw.h"
#include "math.h"
#include <string>

namespace vx {
    auto currentWindowSize() -> ivec2;
    auto launchWindow(const std::string &windowTitle) -> int;
}// namespace vx
