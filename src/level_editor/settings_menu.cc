#include "settings_menu.h"

namespace vx::level_editor {
    void showSettingsMenu() {
        if (ImGui::BeginMenu("Settings")) { ImGui::EndMenu(); }
    }
}// namespace vx::level_editor
