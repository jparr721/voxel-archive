#include "chunk_menu.h"
#include "../gfx/chunk_storage.h"
#include "../gfx/glfw.h"
#include "../gui/input_text.h"
#include "../gui/styles.h"
#include "../util/files.h"
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace vx::level_editor {
    struct ChunkMenuState {
        bool viewChunkPopupOpen = false;
        const gfx::Chunk *selectedChunk = nullptr;

        bool addNewChunkPopupOpen = false;
        bool chunkSettingsMenuVisible = false;
        bool saveButtonDisabled = true;

        const std::string addNewChunkPopupIdentifier = "Add New Chunk";
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

    static void viewChunkPopup() {
        if (chunkMenuState.selectedChunk != nullptr) {
            ImGui::SetNextWindowSize(ImVec2(400, 100));
            if (ImGui::BeginPopupModal(chunkMenuState.selectedChunk->identifier.c_str())) {
                ImGui::Text("N Blocks: %lu", chunkMenuState.selectedChunk->blocks.size());
                ImGui::SameLine();
                ImGui::Text("N Indices: %lu", chunkMenuState.selectedChunk->indices.size());
                ImGui::SameLine();
                ImGui::Text("N Vertices: %lu", chunkMenuState.selectedChunk->geometry.size());
                ImGui::EndPopup();
            }
        }
    }

    static void addNewChunkPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal(chunkMenuState.addNewChunkPopupIdentifier.c_str())) {
            ImGui::Text("Chunk Dimensions");
            ImGui::PushItemWidth(20);
            gui::inputInt("x", "##x", &chunkMenuData.xdim);
            ImGui::PopItemWidth();
            ImGui::PushItemWidth(20);
            gui::inputInt("y", "##y", &chunkMenuData.ydim);
            ImGui::PopItemWidth();
            ImGui::PushItemWidth(20);
            gui::inputInt("z", "##z", &chunkMenuData.zdim);
            ImGui::PopItemWidth();

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

    static void chunkSettingsMenu() {
        GLFWwindow *window = glfwGetCurrentContext();
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        const auto menuBarHeight = ImGui::GetWindowHeight();
        ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(windowWidth * 0.20, windowHeight - menuBarHeight));

        ImGui::Begin("Chunks");
        for (const auto &chunk : gfx::ChunkStorage::getInstance()->chunks()) {
            if (ImGui::Button(chunk.identifier.c_str())) {
                chunkMenuState.selectedChunk = &chunk;
                chunkMenuState.viewChunkPopupOpen = true;
            }
        }
        if (ImGui::Button("Press")) { spdlog::info("Pressed"); }
        ImGui::End();
    }

    void showChunkMenu() {
        chunkMenuState.chunkSettingsMenuVisible = !gfx::ChunkStorage::getInstance()->chunks().empty();
        if (ImGui::BeginMenu("Chunks")) {
            if (ImGui::MenuItem("Add Chunk")) { chunkMenuState.addNewChunkPopupOpen = true; }
            if (ImGui::MenuItem("Toggle Chunks View", nullptr, chunkMenuState.chunkSettingsMenuVisible)) {
                chunkMenuState.chunkSettingsMenuVisible = true;
            }
            ImGui::EndMenu();
        }

        addNewChunkPopup();
        viewChunkPopup();

        if (chunkMenuState.addNewChunkPopupOpen) {
            ImGui::OpenPopup(chunkMenuState.addNewChunkPopupIdentifier.c_str());
            chunkMenuState.addNewChunkPopupOpen = false;
        }

        if (chunkMenuState.viewChunkPopupOpen) {
            ImGui::OpenPopup(chunkMenuState.selectedChunk->identifier.c_str());
            chunkMenuState.viewChunkPopupOpen = false;
        }

        if (chunkMenuState.chunkSettingsMenuVisible) { chunkSettingsMenu(); }
    }
}// namespace vx::level_editor