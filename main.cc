#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

// Logger
#include <spdlog/spdlog.h>

// Geometry
#include "src/geometry.h"
#include "src/math.h"
#include "src/resources.h"
#include "src/trigonometry.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace vx;

static const vec2f kWindowSize(1280, 720);

struct Vertex {
    vec3f position;
    u32 color;
};

static Vertex cubeVertices[] = {
        {vec3f(-1.0f, 1.0f, 1.0f), 0xff000000},   {vec3f(1.0f, 1.0f, 1.0f), 0xff0000ff},
        {vec3f(-1.0f, -1.0f, 1.0f), 0xff00ff00},  {vec3f(1.0f, -1.0f, 1.0f), 0xff00ffff},
        {vec3f(-1.0f, 1.0f, -1.0f), 0xffff0000},  {vec3f(1.0f, 1.0f, -1.0f), 0xffff00ff},
        {vec3f(-1.0f, -1.0f, -1.0f), 0xffffff00}, {vec3f(1.0f, -1.0f, -1.0f), 0xffffffff},
};

static const u16 cubeIndices[] = {
        0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, 1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};


static void glfwErrorCallback(int err, const char *msg) { spdlog::error("GLFW ERROR {}: {}", err, msg); }

int main(int argc, char **argv) {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        spdlog::error("Error initializing GLFW");
        return -1;
    }
    spdlog::info("GLFW Initialized");

    GLFWwindow *window = glfwCreateWindow(kWindowSize.x(), kWindowSize.y(), "GAME", nullptr, nullptr);

    if (!window) {
        spdlog::error("Error creating window");
        glfwTerminate();
        return -1;
    }
    spdlog::info("Window created");

    glfwMakeContextCurrent(window);

    // Tell bgfx to not create a separate render thread
    bgfx::renderFrame();

    bgfx::Init init;

    bgfx::PlatformData platform_data;
    platform_data.nwh = glfwGetCocoaWindow(window);
    init.platformData = platform_data;

    // let system choose renderer
    init.type = bgfx::RendererType::Count;
    init.resolution.width = kWindowSize.x();
    init.resolution.height = kWindowSize.y();
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

    bgfx::reset(kWindowSize.x(), kWindowSize.y(), BGFX_RESET_VSYNC, init.resolution.format);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x003030FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::VertexLayout layout;
    layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .end();

    bgfx::VertexBufferHandle vertex_buffer =
            bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), layout);

    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(cubeIndices, sizeof(cubeIndices)));

    bgfx::ProgramHandle program = loadShaderProgram("resources/shaders", "core", "core.vs.sc", "core.fs.sc");
    spdlog::info("Shader program successfully initialized");

    f32 time = 0.0f;
    const vec3f eye(0.0f, 20.0f, -35.0f);
    const vec3f center(0.0f, 20.0f, -35.0f);
    const vec3f up(0.0f, 20.0f, -35.0f);

    spdlog::info("Beginning render loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::touch(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x6f, "Hello, world");

        mat4f model;
        mat4f view = lookAt(eye, center, up);
        mat4f proj = perspectiveProjection(radians(85.0f), kWindowSize.x() / kWindowSize.y(), 0.01f, 100.0f);

        bgfx::setViewTransform(0, reinterpret_cast<void *>(&view), reinterpret_cast<void *>(&proj));
        bgfx::setViewRect(0, 0, 0, kWindowSize.x(), kWindowSize.y());

        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA |
                    0;// triangle list

        for (uint32_t yy = 0; yy < 11; ++yy) {
            for (uint32_t xx = 0; xx < 11; ++xx) {
                mat4f rotation, translation;
                rotation = rotate(mat4f::Identity(), time + xx * 0.21f, vec3f(1, 0, 0));
                rotation = rotate(rotation, time + yy * 0.37f, vec3f(0, 1, 0));
                translation =
                        translate(mat4f::Identity(), vec3f(-15.0f + (f32) xx * 3.0f, -15.0f + (f32) yy * 3.0f, 0.0f));

                model = translation * rotation;
                bgfx::setTransform(reinterpret_cast<void *>(&model));

                bgfx::setVertexBuffer(0, vertex_buffer);
                bgfx::setIndexBuffer(index_buffer);
                bgfx::setState(state);

                bgfx::submit(0, program);
            }
        }

        bgfx::frame();
        time += 0.01f;
    }

    spdlog::info("Exiting.");
    bgfx::destroy(program);
    bgfx::destroy(vertex_buffer);
    bgfx::destroy(index_buffer);
    bgfx::shutdown();
    glfwTerminate();

    return 0;
}
