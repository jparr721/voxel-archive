#pragma once

#include "math.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <spdlog/spdlog.h>
#include <string>

namespace vx {
    auto loadShader(const std::string &basePath, const std::string &moduleName, const std::string &shaderName)
            -> bgfx::ShaderHandle;
    auto loadShaderProgram(const std::string &basePath, const std::string &moduleName) -> bgfx::ProgramHandle;
}// namespace vx
