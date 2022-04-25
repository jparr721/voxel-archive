#include "window.h"
#include "ctrl/camera.h"
#include "ctrl/input.h"
#include "gfx/block.h"
#include "gfx/primitive.h"
#include "resources.h"
#include "trigonometry.h"
#include "util/colors.h"
#include <spdlog/spdlog.h>


namespace vx {
    static GLFWwindow *window;
    std::vector<gfx::VertexColorHex> cubeVertices = {
            {vec3(0, 0, 0), 0xffffffff}, {vec3(0, 1, 0), 0xffffffff}, {vec3(1, 1, 0), 0xffffffff},
            {vec3(1, 0, 0), 0xffffffff}, {vec3(0, 0, 1), 0xffffffff}, {vec3(0, 1, 1), 0xffffffff},
            {vec3(1, 1, 1), 0xffffffff}, {vec3(1, 0, 1), 0xffffffff},
    };

    static constexpr std::array<u16, 6> cubeIndices{0, 1, 2, 0, 2, 3};

    static std::shared_ptr<ctrl::Camera> camera = std::make_shared<ctrl::Camera>();
    static std::unique_ptr<ctrl::Input> input = std::make_unique<ctrl::Input>();
    std::unique_ptr<gfx::Block> block;

    static void glfwErrorCallback(int err, const char *msg) { spdlog::error("GLFW Error {}: {}", err, msg); }

    static void glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
        input->handleCursorPos(window, xpos, ypos, camera);
    }
    static void glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        input->handleMouseButtonPress(window, button, action, mods, camera);
    }

    static void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        input->handleScrollEvent(xoffset, yoffset, camera);
    }

    auto initializeWindow(const vec2 &windowDimensions, const std::string &windowTitle) -> bool {
        spdlog::info("Loading main window");
        glfwSetErrorCallback(glfwErrorCallback);

        if (!glfwInit()) {
            spdlog::error("Error initializing glfw");
            return false;
        }

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

        return true;
    }

    auto initializeBgfx(const vec2 &windowDimensions, bgfx::VertexLayout &layout, bgfx::ProgramHandle &program)
            -> bool {
        // Tell bgfx to not create a separate render thread
        bgfx::renderFrame();

        bgfx::Init init;

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
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x003030FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#if BX_PLATFORM_WINDOWS
        const std::string shaderResourcePath = "../resources/shaders";
#else
        const std::string shaderResourcePath = "resources/shaders";
#endif

        program = vx::loadShaderProgram(shaderResourcePath, "core", "core.vs.sc", "core.fs.sc");
        return true;
    }

    auto launchWindow(const vec2 &windowDimensions, const std::string &windowTitle) -> int {
        camera->resize(windowDimensions.x, windowDimensions.y);
        initializeWindow(windowDimensions, windowTitle);

        bgfx::VertexLayout layout;
        bgfx::ProgramHandle program;
        initializeBgfx(windowDimensions, layout, program);
        block = std::make_unique<gfx::Block>(gfx::BlockType::kDefault, cubeIndices);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            bgfx::touch(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(0, 0, 0x6f, "");

            bgfx::setViewTransform(0, &camera->viewMatrix(), &camera->projectionMatrix());

            bgfx::setViewRect(0, 0, 0, windowDimensions.x, windowDimensions.y);

            u64 state =
                    BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

            bgfx::setVertexBuffer(0, block->vertexBuffer);
            bgfx::setIndexBuffer(block->indexBuffer);
            bgfx::setState(state);

            bgfx::submit(0, program);

            bgfx::frame();
        }

        bgfx::destroy(program);
        spdlog::info("Deleting buffers");
        bgfx::destroy(block->vertexBuffer);
        bgfx::destroy(block->indexBuffer);
        bgfx::shutdown();
        glfwTerminate();

        return 0;
    }
}// namespace vx
