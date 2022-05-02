#pragma once

#include "../math.h"

namespace vx::ctrl {
    class KeyInput {
    public:
        void handleKeyPressEvent(int key, int scancode, int action, int mods);

    private:
        u8 lastKey;
    };
}// namespace vx::ctrl
