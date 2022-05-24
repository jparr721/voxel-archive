#include "window.h"
#include "ctrl/camera.h"
#include "ctrl/key_input.h"
#include "ctrl/mouse_input.h"
#include "gfx/block.h"
#include "gfx/chunk_storage.h"
#include "gui/menu_bar.h"
#include "gui/styles.h"
#include "imgui_multiplatform/imgui.h"
#include "level_editor/chunk_menu.h"
#include "level_editor/project.h"
#include "level_editor/project_menu.h"
#include "level_editor/settings_menu.h"
#include "paths.h"
#include "resources.h"
#include "trigonometry.h"
#include <imgui_impl_glfw.h>
#include <spdlog/spdlog.h>


namespace vx {
    static GLFWwindow *window;
    static std::shared_ptr<ctrl::Camera> camera = std::make_shared<ctrl::Camera>();
    static std::unique_ptr<ctrl::MouseInput> input = std::make_unique<ctrl::MouseInput>();
    static std::unique_ptr<gui::Menubar> menubar = std::make_unique<gui::Menubar>();
    static bgfx::Init init;
    static ivec2 windowDimensions(1280, 720);

    static void glfwErrorCallback(int err, const char *msg) { spdlog::error("GLFW Error {}: {}", err, msg); }

    static void glfwCursorPosCallback([[maybe_unused]] GLFWwindow *_window, double xpos, double ypos) {
        input->handleCursorPos(xpos, ypos, camera);
    }

    static void glfwMouseButtonCallback(GLFWwindow *_window, int button, int action, int mods) {
        if (!ImGui::GetIO().WantCaptureMouse) { input->handleMouseButtonPress(_window, button, action, mods, camera); }
    }

    static void glfwKeyCallback([[maybe_unused]] GLFWwindow *_window, int key, int scancode, int action, int mods) {
        ctrl::KeyInput::getInstance()->handleKeyPressEvent(key, scancode, action, mods);
    }

    static void glfwScrollCallback([[maybe_unused]] GLFWwindow *_window, double xoffset, double yoffset) {
        input->handleScrollEvent(xoffset, yoffset, camera);
    }

    static void glfwResizeCallback([[maybe_unused]] GLFWwindow *_window, int width, int height) {
        spdlog::debug("Resizing w: {}, h: {}", width, height);
        bgfx::reset(width, height, BGFX_RESET_VSYNC, init.resolution.format);
        camera->resize(width, height);
    }

    static void *glfwNativeWindowHandle(GLFWwindow *_window) {
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#if ENTRY_CONFIG_USE_WAYLAND
        wl_egl_window *winImpl = (wl_egl_window *) glfwGetWindowUserPointer(_window);
        if (!winImpl) {
            int width, height;
            glfwGetWindowSize(_window, &width, &height);
            struct wl_surface *surface = (struct wl_surface *) glfwGetWaylandWindow(_window);
            if (!surface) return nullptr;
            winImpl = wl_egl_window_create(surface, width, height);
            glfwSetWindowUserPointer(_window, (void *) (uintptr_t) winImpl);
        }
        return (void *) (uintptr_t) winImpl;
#else
        return (void *) (uintptr_t) glfwGetX11Window(_window);
#endif
#elif BX_PLATFORM_OSX
        return glfwGetCocoaWindow(_window);
#elif BX_PLATFORM_WINDOWS
        return glfwGetWin32Window(_window);
#endif// BX_PLATFORM_
    }

    static void glfwSetWindow(GLFWwindow *_window) {
        bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#if ENTRY_CONFIG_USE_WAYLAND
        pd.ndt = glfwGetWaylandDisplay();
#else
        pd.ndt = glfwGetX11Display();
#endif
#elif BX_PLATFORM_OSX
        pd.ndt = nullptr;
#elif BX_PLATFORM_WINDOWS
        pd.ndt = nullptr;
#endif// BX_PLATFORM_WINDOWS
        pd.nwh = glfwNativeWindowHandle(_window);
        pd.context = nullptr;
        pd.backBuffer = nullptr;
        pd.backBufferDS = nullptr;
        bgfx::setPlatformData(pd);
    }

    static auto initializeWindow(const std::string &windowTitle) -> bool {
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
        // Set no underlying glfw api
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Set invisible
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, windowTitle.c_str(), nullptr, nullptr);

        if (!window) {
            spdlog::error("Error creating GLFW Window");
            glfwTerminate();
            return false;
        }

        glfwSetCursorPosCallback(window, glfwCursorPosCallback);
        glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
        glfwSetScrollCallback(window, glfwScrollCallback);
        glfwSetWindowSizeCallback(window, glfwResizeCallback);
        glfwSetKeyCallback(window, glfwKeyCallback);
        glfwShowWindow(window);

        return true;
    }

    static auto initializeBgfx() -> bool {
        // Tell bgfx to not create a separate render thread
        bgfx::renderFrame();

        glfwSetWindow(window);

// Use metal for macs and opengl for everything else
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
        imguiCreate();

#ifdef __APPLE__
        ImGui_ImplGlfw_InitForOther(window, true);
#else
        ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif
        return true;
    }

    auto currentWindowSize() -> ivec2 {
        ivec2 res;
        glfwGetWindowSize(window, &res.x, &res.y);
        return res;
    }

    auto launchWindow(const std::string &windowTitle) -> int {
        camera->resize(windowDimensions.x, windowDimensions.y);
        camera->zoom(100);

        // TODO(@jparr721) Make this toggleable
        /* camera->lockCamera(40.0f, 1000.0f, 1.58f, 1.58f, 0.5f, 0.5f); */

        if (!initializeWindow(windowTitle)) {
            spdlog::error("Window initialization failed");
            return EXIT_FAILURE;
        }

        initializeBgfx();

        menubar->registerMenu(level_editor::showProjectMenu);
        menubar->registerMenu(level_editor::showChunkMenu);
        menubar->registerMenu(level_editor::showSettingsMenu);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            //==============================
            const auto currentMousePosition = input->currentMousePos();
            imguiBeginFrame(currentMousePosition.x, currentMousePosition.y, input->mouseButtonImgui(), 0,
                            windowDimensions.x, windowDimensions.y);
            gui::applyDefaultStyle();
            menubar->render(window);
            if (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused()) { ImGui::GetIO().WantCaptureMouse = true; }
            imguiEndFrame();
            //==============================

            bgfx::touch(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(0, 0, 0x6f, "");

            bgfx::setViewTransform(0, &camera->viewMatrix(), &camera->projectionMatrix());

            bgfx::setViewRect(0, 0, 0, windowDimensions.x, windowDimensions.y);

            level_editor::Project::instance()->render();

            bgfx::frame();
        }

        spdlog::info("Deleting buffers");
        imguiDestroy();
        level_editor::Project::instance()->destroy();
        bgfx::shutdown();
        glfwTerminate();

        return 0;
    }
}// namespace vx
