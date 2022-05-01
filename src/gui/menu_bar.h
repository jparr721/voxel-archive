#pragma once

#include "../gfx/glfw.h"
#include <functional>
#include <vector>

namespace vx::gui {
    class Menubar {
    public:
        std::vector<std::function<void()>> callbacks;
        void registerMenu(const std::function<void()> &menuCallbackFn);
        void render(GLFWwindow *window);
    };
}// namespace vx::gui
