#pragma once

#include <string>
#include <vector>

namespace vx::util {
    auto charVecToString(const std::vector<char> &charVec) -> std::string;
    auto stringToCharVec(const std::string &str) -> std::vector<char>;
    auto stringStartsWith(const std::string &word, const std::string &start) -> bool;
    auto stringEndsWith(const std::string &word, const std::string &start) -> bool;
    auto semverToString(int major, int minor, int patch) -> std::string;
}// namespace vx::util
