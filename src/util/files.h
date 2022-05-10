#pragma once

#include <string>

namespace vx::util {
    auto openFileDialog() -> std::string;
    auto openFileDialogAndSave(const std::string &baseName = "") -> std::string;
}// namespace vx::util