#pragma once

#include "gfx/bgfx.h"
#include "gfx/glfw.h"
#include "math.h"
#include <string>

namespace vx {
    auto initializeWindow(const std::string &windowTitle) -> bool;
    auto initializeBgfx() -> bool;
    auto launchWindow(const std::string &windowTitle) -> int;
}// namespace vx
