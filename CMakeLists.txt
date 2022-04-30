cmake_minimum_required(VERSION 3.12...3.18)
project(voxel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Required for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(GLFW_INSTALL OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/dear-imgui)

add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/entt)

option(BGFX_BUILD_EXAMPLES "Build bgfx examples." OFF)
add_subdirectory(third_party/bgfx)
add_subdirectory(third_party/glm)

target_link_libraries(imgui bx)

option(VOXEL_TESTS "Build the tests" ON)
if (VOXEL_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

# Pack the extra imgui stuff
file(GLOB IMGUI_MULTIPLATFORM_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_multiplatform/*.h)
file(GLOB IMGUI_MULTIPLATFORM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_multiplatform/*.cpp)

add_library(${PROJECT_LIB}
  src/ctrl/camera.h
  src/ctrl/input.h
  src/ctrl/key.h

  src/gfx/bgfx.h
  src/gfx/glfw.h
  src/gfx/primitive.h
  src/gfx/program.h
  src/gfx/chunk_renderer.h
  src/gfx/chunk.h
  src/gfx/block.h

  src/util/colors.h

  src/fixtures/fixture.h

  src/geometry.h
  src/math.h
  src/resources.h
  src/trigonometry.h
  src/window.h

  src/ctrl/camera.cc
  src/ctrl/input.cc

  src/gfx/program.cc
  src/gfx/chunk_renderer.cc
  src/gfx/chunk.cc
  src/gfx/block.cc

  src/util/colors.cc

  src/fixtures/fixture.cc

  src/geometry.cc
  src/resources.cc
  src/window.cc

  ${IMGUI_MULTIPLATFORM_HEADERS}
  ${IMGUI_MULTIPLATFORM_SOURCES}
)

target_link_libraries(${PROJECT_LIB} PUBLIC
  imgui
  glfw
  spdlog
  EnTT::EnTT
  bgfx
  bx
  bimg
  glm::glm
)

add_executable(${PROJECT_NAME}
  main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB})

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
