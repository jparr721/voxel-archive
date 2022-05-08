#include "chunk_menu.h"
#include "../gui/input_text.h"
#include "../gui/styles.h"
#include "../util/files.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace vx::level_editor {
    struct ChunkMenuState {
        bool addNewChunkPopupOpen = false;
        bool chunkSettingsMenuVisible = false;
        bool saveButtonDisabled = true;
    };

    struct ChunkMenuData {
        bool isFixture = false;

        // Size of the chunk
        int xdim = 0;
        int ydim = 0;
        int zdim = 0;

        // Offset of the chunk from (0, 0), if a fixture
        int fixtureXOffset = 0;
        int fixtureYOffset = 0;
        int fixtureZOffset = 0;
    };

    static ChunkMenuState chunkMenuState;
    static ChunkMenuData chunkMenuData;

    static void addNewChunkPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal("Add New Chunk")) {
            ImGui::Text("Chunk Dimensions");
            gui::inputInt("x", "##x", &chunkMenuData.xdim);
            gui::inputInt("y", "##y", &chunkMenuData.ydim);
            gui::inputInt("z", "##z", &chunkMenuData.zdim);

            ImGui::Checkbox("Fixture", &chunkMenuData.isFixture);

            if (chunkMenuData.isFixture) {
                ImGui::Text("Offset");
                gui::inputInt("x", "##offsetx", &chunkMenuData.fixtureXOffset);
                gui::inputInt("y", "##offsety", &chunkMenuData.fixtureYOffset);
                gui::inputInt("z", "##offsetz", &chunkMenuData.fixtureZOffset);
            }

            if (chunkMenuState.saveButtonDisabled) { gui::pushDisabled(); }
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                //                ivec3 chunkDimensions =
                //                        ivec3(settingsMenuData.chunksx, settingsMenuData.chunksy, settingsMenuData.chunksz);
                const auto fileName = util::openFileDialogAndSave();
                spdlog::info("filename {}", fileName);
                ImGui::CloseCurrentPopup();
            }
            if (chunkMenuState.saveButtonDisabled) { gui::popDisabled(); }

            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }

    static void chunkSettingsMenu() {}

    void showChunkMenu() {
        if (ImGui::BeginMenu("Chunks")) {
            if (ImGui::MenuItem("Add Chunk")) { chunkMenuState.addNewChunkPopupOpen = true; }
            if (ImGui::MenuItem("Toggle Chunks View")) { chunkMenuState.chunkSettingsMenuVisible = true; }
            ImGui::EndMenu();
        }

        addNewChunkPopup();

        if (chunkMenuState.addNewChunkPopupOpen) {
            ImGui::OpenPopup("Add New Chunk");
            chunkMenuState.addNewChunkPopupOpen = false;
        }

        if (chunkMenuState.chunkSettingsMenuVisible) { chunkSettingsMenu(); }
    }
}// namespace vx::level_editor