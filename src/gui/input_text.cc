#include "input_text.h"

namespace vx::gui {
    void inputText(const char *label, const char *imguiIdentifier, char *value, int bufSize) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::InputText(imguiIdentifier, value, bufSize);
    }

    void inputInt(const char *label, const char *imguiIdentifier, int *value) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::InputInt(imguiIdentifier, value);
    }

    void inputFlat(const char *label, const char *imguiIdentifier, float *value) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::InputFloat(imguiIdentifier, value);
    }
}// namespace vx::gui
