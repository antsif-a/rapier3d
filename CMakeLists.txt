cmake_minimum_required(VERSION 4.0.2)
project(rapier3d)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_options(-O0 -g)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(${EXTERNAL_DIR}/glfw)

# glm
add_subdirectory(${EXTERNAL_DIR}/glm)

# glad
add_subdirectory(${EXTERNAL_DIR}/glad/cmake glad_cmake)
glad_add_library(gl REPRODUCIBLE
  API gl:core=4.6
  EXTENSIONS
    GL_ARB_bindless_texture
)

# tinyobjloader
add_subdirectory(${EXTERNAL_DIR}/tinyobjloader)

# tinyglft
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
set(TINYGLTF_HEADER_ONLY ON)
set(TINYGLTF_INSTALL OFF)
add_subdirectory(${EXTERNAL_DIR}/tinygltf)

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${EXTERNAL_DIR}/stb)

set(SOURCES
  ${SOURCES_DIR}/main.cpp
  ${SOURCES_DIR}/RenderDriver.cpp
  ${SOURCES_DIR}/window/Window.cpp
  ${SOURCES_DIR}/render/model/Obj.cpp
  ${SOURCES_DIR}/render/model/GLtf.cpp
  ${SOURCES_DIR}/render/gl/Shader.cpp
  ${SOURCES_DIR}/render/gl/VertexArray.cpp
  ${SOURCES_DIR}/render/gl/Texture.cpp
  ${SOURCES_DIR}/scene/ViewCamera.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES}
  # https://github.com/ocornut/imgui/wiki/Getting-Started#compilinglinking
  # ${EXTERNAL_DIR}/imgui/imgui.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
  # ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
  # ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE gl glm glfw tinyobjloader stb tinygltf)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES_DIR}
  # ${EXTERNAL_DIR}/imgui
  # ${EXTERNAL_DIR}/imgui/backends
)
target_precompile_headers(${CMAKE_PROJECT_NAME}
  PUBLIC
    <stb_image.h>
    <tiny_obj_loader.h>
    <tiny_gltf.h>
)

file(GLOB ASSETS ${ASSETS_DIR}/*)
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
