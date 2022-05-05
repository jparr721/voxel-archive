#include "settings_menu.h"
#include "../ctrl/key_input.h"
#include "../gfx/block.h"
#include "../gfx/chunk.h"
#include "../util/strings.h"
#include <cstring>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

namespace vx::level_editor {
    bool addChunkPopupOpen = false;
    bool chunkMenuVisible = false;

    static const std::string nameInputFieldIdentifier = "Name";
    static std::vector<gfx::Chunk> chunks{};

    struct SettingsMenuData {
        std::string name = std::string("New Chunk(") + std::to_string(chunks.size()) + std::string(")");
        int chunksx = 0;
        int chunksy = 0;
        int chunksz = 0;
    };

    static SettingsMenuData settingsMenuData;

    static void addChunkPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal("Add New Chunk")) {
            ctrl::KeyInput::getInstance()->registerImGuiKeyCallback(nameInputFieldIdentifier);
            const auto nameFieldKeyState = ctrl::KeyInput::getInstance()->getCallbackState(nameInputFieldIdentifier);
            settingsMenuData.name = nameFieldKeyState.value().buffer;
            ImGui::InputText(nameInputFieldIdentifier.c_str(), const_cast<char *>(settingsMenuData.name.c_str()), 512);
            ctrl::KeyInput::getInstance()->setFocused(nameInputFieldIdentifier, ImGui::IsItemFocused());

            ImGui::InputInt("Chunk Size X", &settingsMenuData.chunksx);
            ImGui::InputInt("Chunk Size Y", &settingsMenuData.chunksy);
            ImGui::InputInt("Chunk Size Z", &settingsMenuData.chunksz);
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                ctrl::KeyInput::getInstance()->removeImGuiKeyCallback(nameInputFieldIdentifier);
                ivec3 chunkDimensions =
                        ivec3(settingsMenuData.chunksx, settingsMenuData.chunksy, settingsMenuData.chunksz);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }

    static void renderChunkMenu() {
        for (const auto &chunk : chunks) { ImGui::Text("%s", chunk.identifier.c_str()); }
    }

    void showSettingsMenu() {
        if (ImGui::BeginMenu("Chunks")) {
            if (ImGui::MenuItem("Add Chunk")) { addChunkPopupOpen = true; }
            if (ImGui::MenuItem("Toggle Chunks View")) { chunkMenuVisible = true; }
            ImGui::EndMenu();
        }

        addChunkPopup();

        if (addChunkPopupOpen) {
            ImGui::OpenPopup("Add New Chunk");
            addChunkPopupOpen = false;
        }

        if (chunkMenuVisible) { renderChunkMenu(); }
    }
}// namespace vx::level_editor
