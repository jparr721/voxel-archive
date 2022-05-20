#include "project_menu.h"
#include "project.h"
#include <array>
#include <cstring>
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace vx::level_editor {
    struct ProjectMenuState {
        bool changeProjectNamePopupVisible = false;

        char projectNameChangePopupTitle[20] = "Change Project Name";
        std::array<char, 512> tempProjectName;
    };

    static ProjectMenuState projectMenuState;

    void loadProject() {}
    void createNewProject() {}

    void changeProjectNamePopup() {
        ImGui::SetNextWindowSize(ImVec2(200, 125));
        if (ImGui::BeginPopupModal(projectMenuState.projectNameChangePopupTitle, nullptr, ImGuiWindowFlags_NoResize)) {
            // Save the preious name in case cancel is clicked
            const std::string previousName = Project::instance()->name;

            ImGui::Text("Project Name");
            ImGui::InputText("##projectName", projectMenuState.tempProjectName.data(), 512);

            // If save, just close the popup with no changes.
            if (ImGui::Button("Save")) {
                Project::instance()->name = projectMenuState.tempProjectName.data();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                // Copy the name back in case it was changed.
                Project::instance()->name = previousName;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void showProjectMenu() {
        // First load, if the temp name is empty, assign it to the current project name
        if (std::strcmp(projectMenuState.tempProjectName.data(), "") == 0) {
            std::strcpy(projectMenuState.tempProjectName.data(), Project::instance()->name.c_str());
        }

        const std::string projectNameListing = "Project name: " + Project::instance()->name;
        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem(projectNameListing.c_str())) { projectMenuState.changeProjectNamePopupVisible = true; }
            if (ImGui::MenuItem("New")) { spdlog::info("Opening new project"); }
            if (ImGui::MenuItem("Load")) { spdlog::info("Loading project"); }
            ImGui::EndMenu();
        }

        changeProjectNamePopup();

        if (projectMenuState.changeProjectNamePopupVisible) {
            ImGui::OpenPopup(projectMenuState.projectNameChangePopupTitle);
            projectMenuState.changeProjectNamePopupVisible = false;
        }
    }
}// namespace vx::level_editor
