cmake_minimum_required(VERSION 3.12...3.18)
project(voxel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Required for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_GLFW "Build target igl::glfw" ON)
option(LIBIGL_IMGUI "Build target igl::imgui" ON)
include(libigl)

add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/entt)

option( BGFX_BUILD_EXAMPLES             "Build bgfx examples."                          OFF)
add_subdirectory(third_party/bgfx)

add_subdirectory(third_party/glm)

option(VOXEL_TESTS "Build the tests" ON)
if (VOXEL_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()


file(GLOB SOURCES src/*.cc)
file(GLOB HEADERS src/*.h)

add_executable(${PROJECT_NAME}
  src/ctrl/camera.h
  src/ctrl/input.h
  src/ctrl/key.h

  src/gfx/bgfx.h
  src/gfx/glfw.h
  src/gfx/primitive.h
  src/gfx/program.h
  src/gfx/chunk_renderer.h

  src/util/colors.h

  src/geometry.h
  src/math.h
  src/resources.h
  src/trigonometry.h
  src/window.h

  src/ctrl/camera.cc
  src/ctrl/input.cc

  src/gfx/program.cc
  src/gfx/chunk_renderer.cc

  src/util/colors.cc

  src/geometry.cc
  src/resources.cc
  src/window.cc
  main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  igl::glfw
  igl::imgui
  spdlog
  EnTT::EnTT
  bgfx
  bx
  bimg
  glm::glm
)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
