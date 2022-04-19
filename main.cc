#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "src/geometry.h"
#include "src/math.h"
#include "src/resources.h"
#include "src/trigonometry.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

// TODO: move elsewhere
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t usize;
typedef ssize_t isize;

typedef float f32;
typedef double f64;

static const glm::vec2 SIZE = glm::vec2(1280, 720);

struct Vertex {
    vx::vec3f position;
    u32 color;
};

static Vertex cube_vertices[] = {
        {vx::vec3f(-1.0f, 1.0f, 1.0f), 0xff000000},   {vx::vec3f(1.0f, 1.0f, 1.0f), 0xff0000ff},
        {vx::vec3f(-1.0f, -1.0f, 1.0f), 0xff00ff00},  {vx::vec3f(1.0f, -1.0f, 1.0f), 0xff00ffff},
        {vx::vec3f(-1.0f, 1.0f, -1.0f), 0xffff0000},  {vx::vec3f(1.0f, 1.0f, -1.0f), 0xffff00ff},
        {vx::vec3f(-1.0f, -1.0f, -1.0f), 0xffffff00}, {vx::vec3f(1.0f, -1.0f, -1.0f), 0xffffffff},
};

static const u16 cube_indices[] = {
        0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, 1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

static void glfw_error_callback(int err, const char *msg) {
    std::cerr << "GLFW ERROR (" << err << "): " << msg << std::endl;
}

int main(int argc, char *argv[]) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(SIZE.x, SIZE.y, "GAME", nullptr, nullptr);

    if (!window) {
        std::cerr << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // TODO: possibly GLFW/OSX specific
    // tell bgfx to not create a separate render thread
    bgfx::renderFrame();

    bgfx::Init init;

    bgfx::PlatformData platform_data;
    platform_data.nwh = glfwGetCocoaWindow(window);
    init.platformData = platform_data;

    // let system choose renderer
    init.type = bgfx::RendererType::Count;
    init.resolution.width = SIZE.x;
    init.resolution.height = SIZE.y;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

    bgfx::reset(SIZE.x, SIZE.y, BGFX_RESET_VSYNC, init.resolution.format);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x003030FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::VertexLayout layout;
    layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .end();

    bgfx::VertexBufferHandle vertexBuffer =
            bgfx::createVertexBuffer(bgfx::makeRef(cube_vertices, sizeof(cube_vertices)), layout);

    bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(cube_indices, sizeof(cube_indices)));

    bgfx::ProgramHandle program = vx::loadShaderProgram("resources/shaders", "core", "core.vs.sc", "core.fs.sc");

    f32 time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        bgfx::touch(0);

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x6f, "Hello, world");

        vx::mat4f view =
                vx::lookAt(vx::vec3f(0.0f, 20.0f, -35.0f), vx::vec3f(0.0f, 0.0f, 0.0f), vx::vec3f(0.0f, 1.0f, 0.0f));
        vx::mat4f proj = vx::perspective(vx::radians(85.0f), SIZE.x / SIZE.y, 0.01f, 100.0f);

        bgfx::setViewTransform(0, reinterpret_cast<void *>(&view), reinterpret_cast<void *>(&proj));
        bgfx::setViewRect(0, 0, 0, SIZE.x, SIZE.y);

        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA |
                    0;// triangle list

        for (uint32_t yy = 0; yy < 11; ++yy) {
            for (uint32_t xx = 0; xx < 11; ++xx) {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), time + xx * 0.21f, glm::vec3(1, 0, 0));
                rotation = glm::rotate(rotation, time + yy * 0.37f, glm::vec3(0, 1, 0));
                glm::mat4 translation = glm::translate(
                        glm::mat4(1.0f), glm::vec3(-15.0f + (f32) xx * 3.0f, -15.0f + (f32) yy * 3.0f, 0.0f));

                glm::mat4 model = translation * rotation;
                bgfx::setTransform(reinterpret_cast<void *>(&model));

                bgfx::setVertexBuffer(0, vertexBuffer);
                bgfx::setIndexBuffer(index_buffer);
                bgfx::setState(state);

                bgfx::submit(0, program);
            }
        }

        bgfx::frame();
        time += 0.01f;
    }

    bgfx::destroy(program);
    bgfx::destroy(vertexBuffer);
    bgfx::destroy(index_buffer);
    bgfx::shutdown();
    glfwTerminate();

    return 0;
}
