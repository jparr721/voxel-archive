#include "project_menu.h"
#include <cstring>
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace vx::level_editor {
    struct ProjectMenuState {
        bool changeProjectNamePopupVisible = false;

        char *projectNameChangePopupTitle = (char *) "Change Project Name";
        char projectName[512] = "Default";
    };

    static ProjectMenuState projectMenuState;

    void loadProject() {}
    void createNewProject() {}

    void changeProjectNamePopup() {
        ImGui::SetNextWindowSize(ImVec2(200, 125));
        if (ImGui::BeginPopupModal(projectMenuState.projectNameChangePopupTitle, nullptr, ImGuiWindowFlags_NoResize)) {
            const auto previousName = projectMenuState.projectName;
            ImGui::Text("Project Name");
            ImGui::InputText("##projectName", projectMenuState.projectName, 512);

            // TODO(@jparr721) - Make this button green
            if (ImGui::Button("Save")) { ImGui::CloseCurrentPopup(); }
            ImGui::SameLine();
            // TODO (@jparr721) - Make this button red
            if (ImGui::Button("Cancel")) {
                // Copy the name back in case it was changed.
                std::strcpy(projectMenuState.projectName, previousName);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void showProjectMenu() {
        char projectNameListing[512] = "Project Name: ";
        std::strcat(projectNameListing, projectMenuState.projectName);

        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem(projectNameListing)) { projectMenuState.changeProjectNamePopupVisible = true; }
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
