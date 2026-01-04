cmake_minimum_required(VERSION 4.2.1)
project(game)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB ASSETS ${ASSETS_DIR}/*)
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME}
  ${SOURCES_DIR}/main.cpp
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -O3)

target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_23)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    rapier3d)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${SOURCES_DIR})
  
target_precompile_headers(${PROJECT_NAME}
  PUBLIC
    <rapier3d/render/gl/GraphicLibrary.hpp>
    <rapier3d/render/gl/Texture.hpp>
    <rapier3d/render/gl/Buffer.hpp>
    <rapier3d/render/gl/Vertex.hpp>
    <rapier3d/render/gl/VertexArray.hpp>
    <rapier3d/render/gl/Shader.hpp>
    <rapier3d/render/TextureLoader.hpp>)

add_executable(demo
  ${SOURCES_DIR}/demo.cpp
)

target_compile_features(demo
  PRIVATE
    cxx_std_23)
target_link_libraries(demo
  PRIVATE
    rapier3d)