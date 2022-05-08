#pragma once

#include <imgui.h>
#include <imgui_internal.h>

namespace vx::gui {
    void pushDisabled();
    void popDisabled();
    void applyDefaultStyle();
}