#include "window.h"
#include "ctrl/camera.h"
#include "ctrl/input.h"
#include "gfx/primitive.h"
#include "resources.h"
#include "trigonometry.h"
#include "util/colors.h"
#include <spdlog/spdlog.h>


namespace vx {
    static GLFWwindow *window;
    static gfx::VertexColorHex cubeVertices[] = {
            {glm::vec3(-1.0f, 1.0f, 1.0f), 0xff000000},   {glm::vec3(1.0f, 1.0f, 1.0f), 0xff0000ff},
            {glm::vec3(-1.0f, -1.0f, 1.0f), 0xff00ff00},  {glm::vec3(1.0f, -1.0f, 1.0f), 0xff00ffff},
            {glm::vec3(-1.0f, 1.0f, -1.0f), 0xffff0000},  {glm::vec3(1.0f, 1.0f, -1.0f), 0xffff00ff},
            {glm::vec3(-1.0f, -1.0f, -1.0f), 0xffffff00}, {glm::vec3(1.0f, -1.0f, -1.0f), 0xffffffff},
    };

    static const u16 cubeIndices[] = {
            0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, 1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
    };

    static std::shared_ptr<ctrl::Camera> camera = std::make_shared<ctrl::Camera>();
    static std::unique_ptr<ctrl::Input> input = std::make_unique<ctrl::Input>();

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

    auto initializeBgfx(const vec2 &windowDimensions, bgfx::VertexLayout &layout,
                        bgfx::VertexBufferHandle &vertexBuffer, bgfx::IndexBufferHandle &indexBuffer,
                        bgfx::ProgramHandle &program) -> bool {
        // Tell bgfx to not create a separate render thread
        bgfx::renderFrame();

        bgfx::Init init;

        bgfx::PlatformData platform_data;
        platform_data.nwh = glfwGetCocoaWindow(window);
        init.platformData = platform_data;

        // let system choose renderer
        init.type = bgfx::RendererType::Count;
        init.resolution.width = windowDimensions.x;
        init.resolution.height = windowDimensions.y;
        init.resolution.reset = BGFX_RESET_VSYNC;
        bgfx::init(init);

        bgfx::reset(windowDimensions.x, windowDimensions.y, BGFX_RESET_VSYNC, init.resolution.format);
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x003030FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

        layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
                .end();

        vertexBuffer = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), layout);

        indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(cubeIndices, sizeof(cubeIndices)));

        program = vx::loadShaderProgram("resources/shaders", "core", "core.vs.sc", "core.fs.sc");
        return true;
    }

    auto launchWindow(const vec2 &windowDimensions, const std::string &windowTitle) -> int {
        camera->resize(windowDimensions.x, windowDimensions.y);
        initializeWindow(windowDimensions, windowTitle);
        bgfx::VertexLayout layout;
        bgfx::VertexBufferHandle vertexBuffer;
        bgfx::IndexBufferHandle indexBuffer;
        bgfx::ProgramHandle program;
        initializeBgfx(windowDimensions, layout, vertexBuffer, indexBuffer, program);

        f32 time = 0.0f;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            bgfx::touch(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(0, 0, 0x6f, "Hello, world");

            bgfx::setViewTransform(0, &camera->viewMatrix(), &camera->projectionMatrix());

            bgfx::setViewRect(0, 0, 0, windowDimensions.x, windowDimensions.y);

            u64 state =
                    BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

            bgfx::setVertexBuffer(0, vertexBuffer);
            bgfx::setIndexBuffer(indexBuffer);
            bgfx::setState(state);

            bgfx::submit(0, program);

            bgfx::frame();
            time += 0.01f;
        }

        bgfx::destroy(program);
        bgfx::destroy(vertexBuffer);
        bgfx::destroy(indexBuffer);
        bgfx::shutdown();
        glfwTerminate();

        return 0;
    }
}// namespace vx
