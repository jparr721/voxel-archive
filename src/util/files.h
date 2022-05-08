#pragma once

#include <string>

namespace vx::util {
    auto openFileDialog() -> std::string;
    auto openFileDialogAndSave() -> std::string;
}// namespace vx::util