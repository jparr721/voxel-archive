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
add_subdirectory(third_party/bgfx)

option(VOXEL_TESTS "Build the tests" ON)
if (VOXEL_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()


file(GLOB SOURCES src/*.cc)
file(GLOB HEADERS src/*.h)

add_executable(${PROJECT_NAME}
  ${SOURCES}
  ${HEADERS}
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
)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
