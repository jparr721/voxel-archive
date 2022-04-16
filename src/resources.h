#pragma once

#include "bx/file.h"
#include "bx/readerwriter.h"
#include "math.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <spdlog/spdlog.h>
#include <string>

namespace vx {
    auto loadMemory(bx::FileReaderI *reader, const std::string &filePath) -> const bgfx::Memory *;
    auto loadShader(const std::string &basePath, const std::string &moduleName, const std::string &shaderName)
            -> bgfx::ShaderHandle;
    auto loadShaderProgram(const std::string &basePath, const std::string &moduleName,
                           const std::string &vertexShaderName, const std::string &fragmentShaderName)
            -> bgfx::ProgramHandle;
}// namespace vx
