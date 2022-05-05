#pragma once

#include <string>
#include <vector>

namespace vx::util {
    auto charVecToString(const std::vector<char> &charVec) -> std::string;
    auto stringToCharVec(const std::string &str) -> std::vector<char>;
    auto mapGLFWInputToChar(int glfwInput, int mods) -> char;
}// namespace vx::util