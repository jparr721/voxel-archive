#pragma once

#include "../math.h"
#include "../window.h"
#include "button.h"
#include "camera.h"
#include "key.h"
#include <array>
#include <memory>

namespace vx::ctrl {
    class MouseInput {
    public:
        MouseInput() : width(1280), height(720), wheelDelta(120), lock(false) {
            for (auto &bs : buttonStates) { bs = 0; }
        }

        void reset();
        void setResolution(i16 _width, i16 _height);
        void setMousePos(const vec3 &_mousePos);
        void setMousePos(i32 x, i32 y, i32 z);
        void setButtonState(MouseButton::Enum button, u8 state);

        bool lock;

        std::array<u8, MouseButton::kCount> buttonStates;

        i32 wheel;

        u16 wheelDelta;
        u16 width;
        u16 height;

        vec3 mousePos;
        vec3 normalizedMousePos;
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
