#include "strings.h"
#include "../gfx/glfw.h"

namespace vx::util {
    auto charVecToString(const std::vector<char> &charVec) -> std::string { return {charVec.begin(), charVec.end()}; }
    auto stringToCharVec(const std::string &str) -> std::vector<char> { return {str.begin(), str.end()}; }
    auto mapGLFWInputToChar(int glfwInput, int mods) -> char {
        const auto shiftKeyPressed = mods == GLFW_MOD_SHIFT;
        switch (glfwInput) {
            case GLFW_KEY_A:
                if (shiftKeyPressed) { return 'A'; }
                return 'a';
            case GLFW_KEY_B:
                if (shiftKeyPressed) { return 'B'; }
                return 'b';
            case GLFW_KEY_C:
                if (shiftKeyPressed) { return 'C'; }
                return 'c';
            case GLFW_KEY_D:
                if (shiftKeyPressed) { return 'D'; }
                return 'd';
            case GLFW_KEY_E:
                if (shiftKeyPressed) { return 'E'; }
                return 'e';
            case GLFW_KEY_F:
                if (shiftKeyPressed) { return 'F'; }
                return 'f';
            case GLFW_KEY_G:
                if (shiftKeyPressed) { return 'G'; }
                return 'g';
            case GLFW_KEY_H:
                if (shiftKeyPressed) { return 'H'; }
                return 'h';
            case GLFW_KEY_I:
                if (shiftKeyPressed) { return 'I'; }
                return 'i';
            case GLFW_KEY_J:
                if (shiftKeyPressed) { return 'J'; }
                return 'j';
            case GLFW_KEY_K:
                if (shiftKeyPressed) { return 'K'; }
                return 'k';
            case GLFW_KEY_L:
                if (shiftKeyPressed) { return 'L'; }
                return 'l';
            case GLFW_KEY_M:
                if (shiftKeyPressed) { return 'M'; }
                return 'm';
            case GLFW_KEY_N:
                if (shiftKeyPressed) { return 'N'; }
                return 'n';
            case GLFW_KEY_O:
                if (shiftKeyPressed) { return 'O'; }
                return 'o';
            case GLFW_KEY_P:
                if (shiftKeyPressed) { return 'P'; }
                return 'p';
            case GLFW_KEY_Q:
                if (shiftKeyPressed) { return 'Q'; }
                return 'q';
            case GLFW_KEY_R:
                if (shiftKeyPressed) { return 'R'; }
                return 'r';
            case GLFW_KEY_S:
                if (shiftKeyPressed) { return 'S'; }
                return 's';
            case GLFW_KEY_T:
                if (shiftKeyPressed) { return 'T'; }
                return 't';
            case GLFW_KEY_U:
                if (shiftKeyPressed) { return 'U'; }
                return 'u';
            case GLFW_KEY_V:
                if (shiftKeyPressed) { return 'V'; }
                return 'v';
            case GLFW_KEY_W:
                if (shiftKeyPressed) { return 'W'; }
                return 'w';
            case GLFW_KEY_X:
                if (shiftKeyPressed) { return 'X'; }
                return 'x';
            case GLFW_KEY_Y:
                if (shiftKeyPressed) { return 'Y'; }
                return 'y';
            case GLFW_KEY_Z:
                if (shiftKeyPressed) { return 'Z'; }
                return 'z';
            default:
                return (char) glfwInput;
        }
    }
}// namespace vx::util
