#pragma once

#include <imgui.h>

namespace vx::gui {
    void inputText(const char *label, const char *imguiIdentifier, char *value, int bufSize);
    void inputInt(const char *label, const char *imguiIdentifier, int *value);
    void inputFloat(const char* imguiIdentifier, float* value);
}
