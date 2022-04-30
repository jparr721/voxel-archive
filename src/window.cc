#include "window.h"
#include "ctrl/camera.h"
#include "ctrl/input.h"
#include "fixtures/fixture.h"
#include "gfx/block.h"
#include "gfx/chunk_renderer.h"
#include "gfx/primitive.h"
#include "imgui_multiplatform/imgui.h"
#include "resources.h"
#include "trigonometry.h"
#include "util/colors.h"
#include <spdlog/spdlog.h>


namespace vx {
    static GLFWwindow *window;
    static std::shared_ptr<ctrl::Camera> camera = std::make_shared<ctrl::Camera>();
    static std::unique_ptr<ctrl::Input> input = std::make_unique<ctrl::Input>();
    static bgfx::Init init;
    static ivec2 windowDimensions(1280, 720);

    static void glfwErrorCallback(int err, const char *msg) { spdlog::error("GLFW Error {}: {}", err, msg); }

    static void glfwCursorPosCallback(GLFWwindow *_window, double xpos, double ypos) {
        input->handleCursorPos(_window, xpos, ypos, camera);
    }

    static void glfwMouseButtonCallback(GLFWwindow *_window, int button, int action, int mods) {
        input->handleMouseButtonPress(_window, button, action, mods, camera);
    }

    static void glfwKeyCallback(GLFWwindow *_window, int key, int scancode, int action, int mods) {
        // TODO (@jparr721) Handle key input

        // If escape, close the window
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(_window, GL_TRUE); }
    }

    static void glfwScrollCallback(GLFWwindow *_window, double xoffset, double yoffset) {
        input->handleScrollEvent(xoffset, yoffset, camera);
    }

    static void glfwResizeCallback(GLFWwindow *window, int width, int height) {
        bgfx::reset(width, height, BGFX_RESET_VSYNC, init.resolution.format);
        camera->resize(width, height);
    }

    auto initializeWindow(const std::string &windowTitle) -> bool {
        spdlog::info("Loading main window");
        glfwSetErrorCallback(glfwErrorCallback);

        if (!glfwInit()) {
            spdlog::error("Error initializing glfw");
            return false;
        }

        // Turn off resize
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();

        if (!monitor) {
            spdlog::error("Could not find primary monitor");
            return false;
        }

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        if (!mode) {
            spdlog::error("Could not get video mode");
            return false;
        }
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        windowDimensions = ivec2(mode->width, mode->height);
        window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, windowTitle.c_str(), nullptr, nullptr);

        if (!window) {
            spdlog::error("Error creating GLFW Window");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);

        glfwSetCursorPosCallback(window, glfwCursorPosCallback);
        glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
        glfwSetScrollCallback(window, glfwScrollCallback);
        glfwSetWindowSizeCallback(window, glfwResizeCallback);
        glfwSetKeyCallback(window, glfwKeyCallback);

        return true;
    }

    auto initializeBgfx(bgfx::ProgramHandle &program) -> bool {
        // Tell bgfx to not create a separate render thread
        bgfx::renderFrame();

        bgfx::PlatformData platformData;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#if ENTRY_CONFIG_USE_WAYLAND
        platformData.ndt = glfwGetWaylandDisplay();
#else
        platformData.ndt = glfwGetX11Display();
        platformData.nwh = (void *) glfwGetX11Window(window);
#endif

#elif BX_PLATFORM_OSX
        platformData.ndt = nullptr;
        platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
        platformData.ndt = nullptr;
        platformData.nwh = glfwGetWin32Window(window);
#endif

        init.platformData = platformData;

// Use metal for macs and vulkan for everything else
#ifdef __APPLE__
        init.type = bgfx::RendererType::Metal;
#else
        init.type = bgfx::RendererType::OpenGL;
#endif
        init.resolution.width = windowDimensions.x;
        init.resolution.height = windowDimensions.y;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        bgfx::reset(windowDimensions.x, windowDimensions.y, BGFX_RESET_VSYNC, init.resolution.format);
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x32323232, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#if BX_PLATFORM_WINDOWS
        const std::string shaderResourcePath = "../resources/shaders";
#else
        const std::string shaderResourcePath = "resources/shaders";
#endif

        program = vx::loadShaderProgram(shaderResourcePath, "core");
        imguiCreate();
        return true;
    }

    auto launchWindow(const std::string &windowTitle) -> int {
        camera->resize(windowDimensions.x, windowDimensions.y);
        if (!initializeWindow(windowTitle)) {
            spdlog::error("Window initialization failed");
            return EXIT_FAILURE;
        }

        bgfx::ProgramHandle program;
        if (!initializeBgfx(program)) {
            spdlog::error("Renderer initialization failed");
            return EXIT_FAILURE;
        }

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            //==============================
            const auto currentMousePosition = input->currentMousePos();
            imguiBeginFrame(currentMousePosition.x, currentMousePosition.y, input->mouseButtonImgui(), 0,
                            uint16_t(windowDimensions.x), uint16_t(windowDimensions.y));
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(windowDimensions.x / 5, windowDimensions.y), ImGuiCond_FirstUseEver);
            ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize);
            if (ImGui::Button("Save")) { spdlog::info("Pressed"); }

            ImGui::End();

            imguiEndFrame();
            //==============================

            bgfx::touch(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(0, 0, 0x6f, "");

            bgfx::setViewTransform(0, &camera->viewMatrix(), &camera->projectionMatrix());

            bgfx::setViewRect(0, 0, 0, windowDimensions.x, windowDimensions.y);

            //! THIS CAUSES A SEGFAULT
            // fixtures::getBaseLayerFixture().renderer->render(program);

            bgfx::frame();
        }

        bgfx::destroy(program);
        spdlog::info("Deleting buffers");
        imguiDestroy();

        //! THIS CAUSES A SEGFAULT
        // fixtures::getBaseLayerFixture().renderer->destroy();
        bgfx::shutdown();
        glfwTerminate();

        return 0;
    }
}// namespace vx
