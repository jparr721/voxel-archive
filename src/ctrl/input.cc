#include "input.h"

namespace vx::ctrl {
    void Input::handleScrollEvent(float xoffset, float yoffset, std::shared_ptr<Camera> &camera_) {
        camera_->zoom(-yoffset);
    }

    void Input::handleMouseButtonPress(GLFWwindow *window, int button, int action, int mods,
                                       std::shared_ptr<Camera> &camera_) {

        camera_->rotating = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        camera_->panning = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    }

    void Input::handleCursorPos(GLFWwindow *window, int xpos, int ypos, std::shared_ptr<Camera> &camera_) {
        current_mouse_pos_ = vec2(xpos, ypos);
        vec2 dpos = last_mouse_pos_ - current_mouse_pos_;

        if (camera_->panning) { camera_->pan(dpos[0], dpos[1]); }
        if (camera_->rotating) { camera_->rotate(dpos[0], dpos[1]); }
        if (camera_->zooming) { camera_->zoom(-dpos[1]); }
        last_mouse_pos_ = current_mouse_pos_;
    }

    void KeyboardInput::reset() {
        for (auto &ks : keyStates) { ks = 0; }
        for (auto &mks : modifierKeyStates) { mks = 0; }
    }

    void KeyboardInput::setKeyState(Key::Enum key, u8 state) { keyStates.at(key) = Key::kNone; }

    void KeyboardInput::setKeyState(Modifier::Enum key, u8 state) { modifierKeyStates.at(key) = Modifier::kNone; }
}// namespace vx::ctrl
