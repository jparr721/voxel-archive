#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>

/* #include "src/camera.h" */
#include "src/geometry.h"
/* #include "src/input.h" */
#include "src/math.h"
#include "src/resources.h"
#include "src/trigonometry.h"
#include "src/window.h"

static const glm::vec2 SIZE = glm::vec2(1280, 720);
/* static void glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos) { */
/*     input.handleCursorPos(window, xpos, ypos, camera); */
/* } */
/* static void glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) { */
/*     input.handleMouseButtonPress(window, button, action, mods, camera); */
/* } */

/* static void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset) { */
/*     input.handleScrollEvent(xoffset, yoffset, camera); */
/* } */

/* glfwSetCursorPosCallback(window, glfwCursorPosCallback); */
/* glfwSetMouseButtonCallback(window, glfwMouseButtonCallback); */
/* glfwSetScrollCallback(window, glfwScrollCallback); */

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif
    vx::launchWindow(SIZE, "Voxel");
}
