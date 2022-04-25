#pragma once

#include "gfx/bgfx.h"
#include "gfx/glfw.h"
#include "math.h"
#include <string>

namespace vx {
    auto initializeWindow(const vec2 &windowDimensions, const std::string &windowTitle) -> bool;
    auto initializeBgfx(const vec2 &windowDimensions, bgfx::VertexLayout &layout,
                        bgfx::DynamicVertexBufferHandle &vertexBuffer, bgfx::DynamicIndexBufferHandle &indexBuffer,
                        bgfx::ProgramHandle &program) -> bool;
    auto launchWindow(const vec2 &windowDimensions, const std::string &windowTitle) -> int;
}// namespace vx
