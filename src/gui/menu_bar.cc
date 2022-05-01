#include "menu_bar.h"
#include "imgui.h"

namespace vx::gui {
    void Menubar::registerMenu(const std::function<void()> &menuCallbackFn) { callbacks.push_back(menuCallbackFn); }
    void Menubar::render(GLFWwindow *window) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Voxel")) {
                if (ImGui::MenuItem("Close")) { glfwSetWindowShouldClose(window, GL_TRUE); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save")) {}
                ImGui::EndMenu();
            }

            for (const auto &callback : callbacks) { callback(); }

            ImGui::EndMainMenuBar();
        }
    }
}// namespace vx::gui
