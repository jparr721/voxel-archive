#include "input.h"

namespace vx::ctrl {
    void MouseInput::reset() {
        if (lock) { normalizedMousePos = vec3(0); }
        for (auto &bs : buttonStates) { bs = MouseButton::kNone; }
    }

    void MouseInput::setResolution(i16 _width, i16 _height) {
        width = _width;
        height = _height;
    }

    void MouseInput::setMousePos(const vec3 &_mousePos) { mousePos = _mousePos; }

    void MouseInput::setMousePos(i32 x, i32 y, i32 z) { mousePos = vec3(x, y, z); }

    void MouseInput::setButtonState(MouseButton::Enum button, u8 state) { buttonStates.at(button) = state; }

    void KeyboardInput::reset() {
        for (auto &ks : keyStates) { ks = 0; }
        for (auto &mks : modifierKeyStates) { mks = 0; }
    }

    void KeyboardInput::setKeyState(Key::Enum key, u8 state) { keyStates.at(key) = Key::kNone; }

    void KeyboardInput::setKeyState(Modifier::Enum key, u8 state) { modifierKeyStates.at(key) = Modifier::kNone; }
}// namespace vx::ctrl
