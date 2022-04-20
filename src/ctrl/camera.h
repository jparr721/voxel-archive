#pragma once

#include "../math.h"
#include "button.h"
#include <Eigen/Dense>
#include <array>


namespace vx::ctrl {
    constexpr u8 kCameraKeyForward = 0x01;
    constexpr u8 kCameraKeyBackward = 0x02;
    constexpr u8 kCameraKeyLeft = 0x04;
    constexpr u8 kCameraKeyRight = 0x08;
    constexpr u8 kCameraKeyUp = 0x10;
    constexpr u8 kCameraKeyDown = 0x20;

    struct MouseState {
        MouseState() : mx(0), my(0), mz(0) {
            for (uint32_t ii = 0; ii < MouseButton::kCount; ++ii) { buttons[ii] = MouseButton::kNone; }
        }

        i32 mx;
        i32 my;
        i32 mz;
        std::array<i32, MouseButton::kCount> buttons;
    };

    class Camera {
    public:
    private:
    };

}// namespace vx::ctrl
