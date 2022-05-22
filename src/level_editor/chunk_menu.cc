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
        bool chunkSettingsMenuVisible = true;
        bool addAnotherChunk = false;
        bool editingExistingChunk = false;
        bool nameAlreadyTaken = false;

        bool saveButtonDisabled = true;

        int selectedShaderModuleOption = 0;

        const std::string addNewChunkPopupIdentifier = "Add New Chunk";
    };

    struct ChunkMenuData {
        char identifier[512];
        std::string shaderModule = "core";

        bool isFixture = true;

        // Size of the chunk
        int xdim = 1;
        int ydim = 1;
        int zdim = 1;

        // Offset of the chunk from (0, 0), if a fixture
        int fixtureXOffset = 0;
        int fixtureYOffset = 0;
        int fixtureZOffset = 0;

        auto operator=(const gfx::Chunk &chunk) -> ChunkMenuData & {
            std::strcpy(identifier, chunk.identifier.c_str());
            shaderModule = chunk.shaderModule;

            xdim = chunk.xdim;
            ydim = chunk.ydim;
            zdim = chunk.zdim;

            fixtureXOffset = chunk.geometry.front().position.x;
            fixtureYOffset = chunk.geometry.front().position.y;
            fixtureZOffset = chunk.geometry.front().position.z;
            return *this;
        }
    };

    static ChunkMenuState chunkMenuState;
    static ChunkMenuData chunkMenuData;

    static auto isInvalidChunkSize(int x, int y, int z) -> bool {
        const bool tooLarge =
                ((x * y * z) * gfx::kCubeVertices.size()) > std::numeric_limits<gfx::BlockIndexSize>::max();
        const bool invalidValue = x == 0 || y == 0 || z == 0;
        return tooLarge || invalidValue;
    }

    static void chunkEditPopup() {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        if (ImGui::BeginPopupModal(chunkMenuState.addNewChunkPopupIdentifier.c_str())) {
            ImGui::Text("Identifier");
            ImGui::InputText("##identifier", chunkMenuData.identifier, 512);

            // Check if the naem is already taken
            const std::function<bool(const gfx::Chunk &, const std::string &)> compare =
                    [&](const gfx::Chunk &chunk, const std::string &identifier) {
                        if (chunk.identifier == identifier) { return true; }
                        return false;
                    };
            chunkMenuState.nameAlreadyTaken = util::inContainer(level_editor::Project::instance()->storage()->chunks(),
                                                                std::string(chunkMenuData.identifier), compare) > -1;
            if (chunkMenuState.nameAlreadyTaken) { ImGui::TextColored(ImVec4(1, 0, 0, 1), "Name is taken"); }

            // Load shader modules from disk in the resource path for slection
            ImGui::Text("Shader Module");
            ImGui::Combo("##shadermodule", &chunkMenuState.selectedShaderModuleOption,
                         paths::kAvailableShaderModules.data(), 2);
            chunkMenuData.shaderModule = paths::kAvailableShaderModules.at(chunkMenuState.selectedShaderModuleOption);

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
                ImGui::InputInt("##xoffset", &chunkMenuData.fixtureXOffset, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##yoffset", &chunkMenuData.fixtureYOffset, 5);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(tripletInputWidth);
                ImGui::InputInt("##zoffset", &chunkMenuData.fixtureZOffset, 5);
            }

            ImGui::Checkbox("Add Another Chunk", &chunkMenuState.addAnotherChunk);

            chunkMenuState.saveButtonDisabled = chunkMenuState.nameAlreadyTaken || chunkSizeInvalid;
            if (chunkMenuState.saveButtonDisabled) { gui::pushDisabled(); }
            // TODO(@jparr721) - Make this button green
            if (ImGui::Button("Save", ImVec2(ImGui::GetWindowSize().x * 0.5f, 0.0f))) {
                const ivec3 chunkDimensions(chunkMenuData.xdim, chunkMenuData.ydim, chunkMenuData.zdim);
                const vec3 chunkTranslation(chunkMenuData.fixtureXOffset, chunkMenuData.fixtureYOffset,
                                            chunkMenuData.fixtureZOffset);

                const gfx::Chunk chunk(chunkDimensions, chunkTranslation, chunkMenuData.shaderModule,
                                       chunkMenuData.identifier, chunkMenuData.isFixture);
                if (chunkMenuState.editingExistingChunk) {
                    level_editor::Project::instance()->setChunk(chunk);
                    chunkMenuState.editingExistingChunk = false;
                } else {
                    level_editor::Project::instance()->addChunk(chunk);
                }

                if (!chunkMenuState.addAnotherChunk) {
                    ImGui::CloseCurrentPopup();
                } else {
                    chunkMenuData.shaderModule = "core";
                    chunkMenuState.selectedShaderModuleOption = 0;
                    chunkMenuData.isFixture = true;
                    chunkMenuData.xdim = 0;
                    chunkMenuData.ydim = 0;
                    chunkMenuData.zdim = 0;
                    chunkMenuData.fixtureXOffset = 0;
                    chunkMenuData.fixtureYOffset = 0;
                    chunkMenuData.fixtureZOffset = 0;
                }
            }
            if (chunkMenuState.saveButtonDisabled) { gui::popDisabled(); }

            ImGui::SameLine();
            // TODO (@jparr721) - Make this button red
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
            for (const auto &chunk : level_editor::Project::instance()->getChunks()) {
                if (ImGui::TreeNodeEx(chunk.identifier.c_str())) {
                    ImGui::Text("N Indices: %lu", chunk.indices.size());
                    ImGui::Text("Max Index: %u", *std::max_element(chunk.indices.begin(), chunk.indices.end()));
                    ImGui::Text("N Vertices: %lu", chunk.geometry.size());
                    ImGui::Text("X Bounds: %i", chunk.xdim);
                    ImGui::Text("Y Bounds: %i", chunk.ydim);
                    ImGui::Text("Z Bounds: %i", chunk.zdim);
                    ImGui::Text("Starting Point %s", glm::to_string(ivec3(chunk.geometry.front().position)).c_str());

                    if (ImGui::Button("Edit")) {
                        chunkMenuData = chunk;
                        chunkMenuState.addNewChunkPopupOpen = true;
                        chunkMenuState.editingExistingChunk = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Delete")) { level_editor::Project::instance()->deleteChunk(chunk); }
                    ImGui::TreePop();
                }
            }
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

        chunkEditPopup();

        if (chunkMenuState.addNewChunkPopupOpen) {
            ImGui::OpenPopup(chunkMenuState.addNewChunkPopupIdentifier.c_str());
            chunkMenuState.addNewChunkPopupOpen = false;
        }

        if (chunkMenuState.chunkSettingsMenuVisible) { chunkSettingsMenu(); }
    }
}// namespace vx::level_editor
