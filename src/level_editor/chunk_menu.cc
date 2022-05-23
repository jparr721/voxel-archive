#include "chunk_menu.h"
#include "../gfx/chunk_storage.h"
#include "../gfx/glfw.h"
#include "../gui/styles.h"
#include "../level_editor/project.h"
#include "../paths.h"
#include "../util/collections.h"
#include "../util/files.h"
#include <cstring>
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace vx::level_editor {
    struct ChunkMenuState {
        bool addNewChunkPopupOpen = false;
        bool editChunkPopupOpen = false;
        bool chunkSettingsMenuVisible = true;
        bool addAnotherChunk = false;

        int selectedShaderModuleOption = 0;
        int selectedBlockTypeOption = 0;

        const std::string addNewChunkPopupIdentifier = "Add New Chunk";
        const std::string editChunkPopupIdentifier = "Edit Chunk";

        std::vector<uuids::uuid> deletedChunks;

        gfx::Chunk *editedChunk = nullptr;
    };

    struct ChunkMenuData {
        char chunkName[512];
        std::string shaderModule = "core";
        gfx::BlockType blockType = gfx::BlockType::kDebug;

        bool isFixture = true;

        // Size of the chunk
        int xdim = 1;
        int ydim = 1;
        int zdim = 1;

        // Offset of the chunk from (0, 0), if a fixture
        int xtransform = 0;
        int ytransform = 0;
        int ztransform = 0;
    };

    static ChunkMenuState chunkMenuState;
    static ChunkMenuData chunkMenuData;

    static auto isInvalidChunkSize(int x, int y, int z) -> bool {
        const bool tooLarge =
                ((x * y * z) * gfx::kCubeVertices.size()) > std::numeric_limits<gfx::BlockIndexSize>::max();
        const bool invalidValue = x == 0 || y == 0 || z == 0;
        return tooLarge || invalidValue;
    }

    static void editChunkPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (chunkMenuState.editedChunk) {
            if (ImGui::BeginPopupModal(chunkMenuState.editChunkPopupIdentifier.c_str())) {
                ImGui::Text("Name");
                ImGui::InputText("##name", chunkMenuData.chunkName, 512);

                // Load shader modules from disk in the resource path for slection
                ImGui::Text("Shader Module");
                ImGui::Combo("##shadermodule", &chunkMenuState.selectedShaderModuleOption,
                             paths::kAvailableShaderModules.data(), 2);
                chunkMenuData.shaderModule =
                        paths::kAvailableShaderModules.at(chunkMenuState.selectedShaderModuleOption);

                const auto &[width, height] = ImGui::GetWindowSize();
                const float tripletInputWidth = width * 0.3;

                const auto chunkSizeInvalid =
                        isInvalidChunkSize(chunkMenuData.xdim, chunkMenuData.ydim, chunkMenuData.zdim);
                if (chunkSizeInvalid) { ImGui::TextColored(ImVec4(1, 0, 0, 1), "Chunk size is invalid"); }

                ImGui::Text("Chunk Dimensions (x, y, z)");
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##x", &chunkMenuData.xdim, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##y", &chunkMenuData.ydim, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##z", &chunkMenuData.zdim, 5);

                ImGui::Checkbox("Fixture", &chunkMenuData.isFixture);

                if (chunkMenuData.isFixture) {
                    ImGui::Text("Transform");
                    ImGui::SetNextItemWidth(tripletInputWidth);
                    ImGui::InputInt("##xtransform", &chunkMenuData.xtransform, 5);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(tripletInputWidth);
                    ImGui::InputInt("##ytransform", &chunkMenuData.ytransform, 5);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(tripletInputWidth);
                    ImGui::InputInt("##ztransform", &chunkMenuData.ztransform, 5);
                }

                if (chunkSizeInvalid) { gui::pushDisabled(); }
                if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                    const ivec3 chunkDimensions(chunkMenuData.xdim, chunkMenuData.ydim, chunkMenuData.zdim);
                    const vec3 chunkTranslation(chunkMenuData.xtransform, chunkMenuData.ytransform,
                                                chunkMenuData.ztransform);

                    chunkMenuState.editedChunk->name = chunkMenuData.chunkName;
                    chunkMenuState.editedChunk->shaderModule = chunkMenuData.shaderModule;
                    chunkMenuState.editedChunk->isFixture = chunkMenuData.isFixture;
                    chunkMenuState.editedChunk->setGeometry(chunkDimensions, chunkTranslation, gfx::BlockType::kDebug);

                    // Tell the render step to reload the objects in memory
                    chunkMenuState.editedChunk->needsUpdate = true;
                    ImGui::CloseCurrentPopup();
                }
                if (chunkSizeInvalid) { gui::popDisabled(); }

                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    static void chunkCreatePopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal(chunkMenuState.addNewChunkPopupIdentifier.c_str())) {
            ImGui::Text("Name");
            ImGui::InputText("##name", chunkMenuData.chunkName, 512);

            // Load shader modules from disk in the resource path for slection
            ImGui::Text("Shader Module");
            ImGui::Combo("##shadermodule", &chunkMenuState.selectedShaderModuleOption,
                         paths::kAvailableShaderModules.data(), paths::kAvailableShaderModules.size());
            chunkMenuData.shaderModule = paths::kAvailableShaderModules.at(chunkMenuState.selectedShaderModuleOption);

            ImGui::Text("Block Type");
            ImGui::Combo("##blocktype", &chunkMenuState.selectedBlockTypeOption, gfx::kAvailableBlockTypes.data(),
                         gfx::kAvailableBlockTypes.size());
            chunkMenuData.blockType =
                    gfx::blockTypeFromString(gfx::kAvailableBlockTypes.at(chunkMenuState.selectedBlockTypeOption));

            const auto &[width, height] = ImGui::GetWindowSize();
            const float tripletInputWidth = width * 0.3;

            // TODO(@jparr721) Better error message
            const auto chunkSizeInvalid =
                    isInvalidChunkSize(chunkMenuData.xdim, chunkMenuData.ydim, chunkMenuData.zdim);
            if (chunkSizeInvalid) { ImGui::TextColored(ImVec4(1, 0, 0, 1), "Chunk size is invalid"); }

            ImGui::Text("Chunk Dimensions (x, y, z)");
            ImGui::SetNextItemWidth(tripletInputWidth);
            ImGui::InputInt("##x", &chunkMenuData.xdim, 5);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(tripletInputWidth);
            ImGui::InputInt("##y", &chunkMenuData.ydim, 5);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(tripletInputWidth);
            ImGui::InputInt("##z", &chunkMenuData.zdim, 5);

            ImGui::Checkbox("Fixture", &chunkMenuData.isFixture);

            if (chunkMenuData.isFixture) {
                ImGui::Text("Offset");
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##xoffset", &chunkMenuData.xtransform, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##yoffset", &chunkMenuData.ytransform, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##zoffset", &chunkMenuData.ztransform, 5);
            }

            ImGui::Checkbox("Add Another Chunk", &chunkMenuState.addAnotherChunk);

            if (chunkSizeInvalid) { gui::pushDisabled(); }
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                const ivec3 chunkDimensions(chunkMenuData.xdim, chunkMenuData.ydim, chunkMenuData.zdim);
                const vec3 chunkTranslation(chunkMenuData.xtransform, chunkMenuData.ytransform,
                                            chunkMenuData.ztransform);

                const gfx::Chunk chunk(chunkDimensions, chunkTranslation, chunkMenuData.shaderModule,
                                       chunkMenuData.chunkName, chunkMenuData.isFixture, chunkMenuData.blockType);
                level_editor::Project::instance()->addChunk(chunk);

                if (!chunkMenuState.addAnotherChunk) { ImGui::CloseCurrentPopup(); }
            }
            if (chunkSizeInvalid) { gui::popDisabled(); }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) { ImGui::CloseCurrentPopup(); }
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
        ImGui::PushItemFlag(ImGuiTreeNodeFlags_SpanAvailWidth, true);
        if (level_editor::Project::instance()->storage()->chunks().empty()) {
            ImGui::Text("No Chunks Loaded.");
        } else {
            for (auto &[id, chunk] : level_editor::Project::instance()->getChunks()) {
                if (ImGui::TreeNodeEx(chunk.name.c_str())) {
                    ImGui::Text("id: %s", uuids::to_string(chunk.id).c_str());
                    ImGui::Text("Dims: %i, %i, %i", chunk.xdim, chunk.ydim, chunk.zdim);
                    ImGui::Text("Transform: %i, %i, %i", chunk.xtransform, chunk.ytransform, chunk.ztransform);

                    if (ImGui::Button("Edit")) {
                        chunkMenuState.editChunkPopupOpen = true;
                        chunkMenuState.editedChunk = &chunk;

                        // Pre-fill the data
                        // Assign the name since it's a non std::string type
                        std::strcpy(chunkMenuData.chunkName, chunkMenuState.editedChunk->name.c_str());

                        // Map the selected shader option for the module combo box
                        chunkMenuState.selectedShaderModuleOption =
                                paths::indexOfShaderModule(chunkMenuState.editedChunk->shaderModule);

                        // Set menu dims for overridding later
                        chunkMenuData.xdim = chunkMenuState.editedChunk->xdim;
                        chunkMenuData.ydim = chunkMenuState.editedChunk->ydim;
                        chunkMenuData.zdim = chunkMenuState.editedChunk->zdim;

                        // Set menu transforms for overridding later
                        chunkMenuData.xtransform = chunkMenuState.editedChunk->xtransform;
                        chunkMenuData.ytransform = chunkMenuState.editedChunk->ytransform;
                        chunkMenuData.ztransform = chunkMenuState.editedChunk->ztransform;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Delete")) { chunkMenuState.deletedChunks.push_back(chunk.id); }
                    ImGui::TreePop();
                }
            }

            // Delete chunks
            for (const auto &id : chunkMenuState.deletedChunks) { level_editor::Project::instance()->deleteChunk(id); }
            // Clear the deleted chunks
            chunkMenuState.deletedChunks.clear();
        }
        ImGui::PopItemFlag();
        ImGui::End();
    }

    void showChunkMenu() {
        if (ImGui::BeginMenu("Chunks")) {
            if (ImGui::MenuItem("Add Chunk")) { chunkMenuState.addNewChunkPopupOpen = true; }
            if (ImGui::MenuItem("Toggle Chunks View", nullptr, chunkMenuState.chunkSettingsMenuVisible)) {
                chunkMenuState.chunkSettingsMenuVisible = !chunkMenuState.chunkSettingsMenuVisible;
            }
            ImGui::EndMenu();
        }

        chunkCreatePopup();
        editChunkPopup();

        if (chunkMenuState.addNewChunkPopupOpen) {
            ImGui::OpenPopup(chunkMenuState.addNewChunkPopupIdentifier.c_str());
            chunkMenuState.addNewChunkPopupOpen = false;
        }

        if (chunkMenuState.editChunkPopupOpen) {
            ImGui::OpenPopup(chunkMenuState.editChunkPopupIdentifier.c_str());
            chunkMenuState.editChunkPopupOpen = false;
        }

        if (chunkMenuState.chunkSettingsMenuVisible) { chunkSettingsMenu(); }
    }
}// namespace vx::level_editor
