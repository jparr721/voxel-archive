#include "settings_menu.h"
#include "../gfx/block.h"
#include <spdlog/spdlog.h>
#include <vector>

namespace vx::level_editor {
    bool addChunkPopupOpen = false;
    bool blockMenuVisible = false;
    int nBlocks = 0;
    char name[64] = "New Block";
    static std::vector<gfx::Block> blocks;

    static int imguiInputTextCallback(ImGuiInputTextCallbackData *data) {
        spdlog::info("cursorpos {}", data->CursorPos);
        return 0;
    }

    static void addBlockPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal("Add New Block")) {
            int flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue |
                        ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoHorizontalScroll |
                        ImGuiInputTextFlags_AlwaysOverwrite | ImGuiInputTextFlags_CallbackAlways;

            ImGui::InputText("Name", name, 64, flags, imguiInputTextCallback);
            ImGui::InputInt("N Blocks", &nBlocks);
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                spdlog::info("Name {}", name);
                spdlog::info("n Blocks {}", nBlocks);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }

    static void renderBlockMenu() {
        for (const auto &block : blocks) { ImGui::Text("Block"); }
    }

    void showSettingsMenu() {
        if (ImGui::BeginMenu("Blocks")) {
            if (ImGui::MenuItem("Add Block")) { addChunkPopupOpen = true; }
            if (ImGui::MenuItem("Toggle Blocks View")) { blockMenuVisible = true; }
            ImGui::EndMenu();
        }

        addBlockPopup();

        if (addChunkPopupOpen) {
            ImGui::OpenPopup("Add New Block");
            addChunkPopupOpen = false;
        }
    }
}// namespace vx::level_editor
