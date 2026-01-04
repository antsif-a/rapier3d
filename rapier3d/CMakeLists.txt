cmake_minimum_required(VERSION 4.2.1)
project(rapier3d)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

## external dependencies

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

add_library(${PROJECT_NAME}
  ${SOURCES_DIR}/render/TextureLoader.cpp
  ${SOURCES_DIR}/render/model/Obj.cpp
  ${SOURCES_DIR}/render/model/GLtf.cpp
  ${SOURCES_DIR}/scene/ViewCamera.cpp
  # https://github.com/ocornut/imgui/wiki/Getting-Started#compilinglinking
  # ${EXTERNAL_DIR}/imgui/imgui.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
  # ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
  # ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
  # ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -O3)

target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_23)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    glm rapier3d-gl rapier3d-framework
  PRIVATE
    tinyobjloader stb tinygltf)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${INCLUDE_DIR}
  # ${EXTERNAL_DIR}/imgui
  # ${EXTERNAL_DIR}/imgui/backends  
)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE
    <stb_image.h>
    <tiny_obj_loader.h>
    <tiny_gltf.h>)