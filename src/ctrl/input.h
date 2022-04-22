#pragma once

#include "../math.h"
#include "../window.h"
#include "button.h"
#include "camera.h"
#include "key.h"
#include <array>
#include <memory>

namespace vx::ctrl {
    class Input {
    public:
        void handleScrollEvent(float xoffset, float yoffset, std::shared_ptr<Camera> &camera_);
        void handleMouseButtonPress(GLFWwindow *window, int button, int action, int mods,
                                    std::shared_ptr<Camera> &camera_);
        void handleCursorPos(GLFWwindow *window, int xpos, int ypos, std::shared_ptr<Camera> &camera_);

    private:
        bool mouse_pressed = false;

        vec2 current_mouse_pos_ = vec2(0);
        vec2 last_mouse_pos_ = vec2(0);
    };

    class KeyboardInput {
    public:
        KeyboardInput() {
            for (auto &ks : keyStates) { ks = 0; }
            for (auto &mks : modifierKeyStates) { mks = 0; }
        }

        void reset();
        void setKeyState(Key::Enum key, u8 state);
        void setKeyState(Modifier::Enum key, u8 state);

        std::array<u8, Key::kCount> keyStates;
        std::array<u8, Modifier::kCount> modifierKeyStates;
    };
}// namespace vx::ctrl
