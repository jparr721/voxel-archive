#include "settings_menu.h"
#include "../ctrl/key_input.h"
#include "../gfx/block.h"
#include "../util/strings.h"
#include <cstring>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

namespace vx::level_editor {
    bool addChunkPopupOpen = false;
    bool blockMenuVisible = false;

    static const std::string nameInputFieldIdentifier = "Name";
    static std::vector<gfx::Block> blocks{};

    struct SettingsMenuData {
        std::string name = std::string("New Block(") + std::to_string(blocks.size()) + std::string(")");
        int nblocks = 0;
    };

    static SettingsMenuData settingsMenuData;

    static void addBlockPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal("Add New Block")) {
            ctrl::KeyInput::getInstance()->registerImGuiKeyCallback(nameInputFieldIdentifier);
            const auto nameFieldKeyState = ctrl::KeyInput::getInstance()->getCallbackState(nameInputFieldIdentifier);
            settingsMenuData.name = nameFieldKeyState.value().buffer;
            ImGui::InputText(nameInputFieldIdentifier.c_str(), const_cast<char *>(settingsMenuData.name.c_str()), 512);
            ctrl::KeyInput::getInstance()->setFocused(nameInputFieldIdentifier, ImGui::IsItemFocused());

            ImGui::InputInt("N Blocks", &settingsMenuData.nblocks);
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                ctrl::KeyInput::getInstance()->removeImGuiKeyCallback(nameInputFieldIdentifier);
                spdlog::info("NAME {}", settingsMenuData.name);
                spdlog::info("nblocks {}", settingsMenuData.nblocks);
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
