#include "strings.h"
#include "../gfx/glfw.h"

namespace vx::util {
    auto charVecToString(const std::vector<char> &charVec) -> std::string { return {charVec.begin(), charVec.end()}; }
    auto stringToCharVec(const std::string &str) -> std::vector<char> { return {str.begin(), str.end()}; }

    auto stringEndsWith(const std::string &word, const std::string &end) -> bool {
        if (word.size() < end.size()) { return false; }

        for (int ii = word.size() - end.size(), jj = 0; ii < word.size(); ++ii, ++jj) {
            if (word.at(ii) != end.at(jj)) { return false; }
        }

        return true;
    }

    auto stringStartsWith(const std::string &word, const std::string &start) -> bool {
        if (word.size() > start.size()) { return false; }

        for (int ii = 0; ii < start.size(); ++ii) {
            if (word.at(ii) != start.at(ii)) { return false; }
        }

        return true;
    }
}// namespace vx::util
