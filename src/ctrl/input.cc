#include "input.h"
#include "../imgui_multiplatform/imgui.h"
#include <spdlog/spdlog.h>

namespace vx::ctrl {
    void Input::handleScrollEvent(float xoffset, float yoffset, std::shared_ptr<Camera> &camera_) {
        camera_->zoom(-yoffset);
    }

    void Input::handleMouseButtonPress(GLFWwindow *window, int button, int action, int mods,
                                       std::shared_ptr<Camera> &camera_) {
        mousePressed_ = action == GLFW_PRESS;
        mouseButton_ = button;

        mouseButtonStates_.at(mouseButton_) = mousePressed_;

        camera_->rotating = mouseButton_ == GLFW_MOUSE_BUTTON_LEFT && mousePressed_;
        camera_->panning = mouseButton_ == GLFW_MOUSE_BUTTON_RIGHT && mousePressed_;
    }

    auto Input::mouseButton() -> int {
        if (mousePressed_) { return mouseButton_; }
        return 0;
    }

    auto Input::mouseButtonImgui() -> int {
        return (mouseButtonStates_.at(GLFW_MOUSE_BUTTON_LEFT) ? IMGUI_MBUT_LEFT : 0) |
               (mouseButtonStates_.at(GLFW_MOUSE_BUTTON_RIGHT) ? IMGUI_MBUT_RIGHT : 0) |
               (mouseButtonStates_.at(GLFW_MOUSE_BUTTON_MIDDLE) ? IMGUI_MBUT_MIDDLE : 0);
    }

    void Input::handleCursorPos(GLFWwindow *window, int xpos, int ypos, std::shared_ptr<Camera> &camera_) {
        currentMousePos_ = vec2(xpos, ypos);
        vec2 dpos = lastMousePos_ - currentMousePos_;

        if (camera_->panning) { camera_->pan(dpos[0], dpos[1]); }
        if (camera_->rotating) { camera_->rotate(dpos[0], dpos[1]); }
        if (camera_->zooming) { camera_->zoom(-dpos[1]); }
        lastMousePos_ = currentMousePos_;
    }
}// namespace vx::ctrl
