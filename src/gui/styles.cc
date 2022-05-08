#include "styles.h"

namespace vx::gui {
    void pushDisabled() {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true /* enabled */);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    void popDisabled() {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
}// namespace vx::gui
