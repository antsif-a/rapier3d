cmake_minimum_required(VERSION 4.2.1)
project(rapier3d-gl)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# glad
add_subdirectory(${EXTERNAL_DIR}/glad/cmake glad_cmake)
glad_add_library(gl REPRODUCIBLE
  API gl:core=4.6
  EXTENSIONS
    GL_ARB_bindless_texture
)

add_library(${PROJECT_NAME}
  ${SOURCES_DIR}/Debug.cpp
  ${SOURCES_DIR}/Shader.cpp
  ${SOURCES_DIR}/Texture.cpp
  ${SOURCES_DIR}/VertexArray.cpp
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -O3)

target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_23)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    gl
  PRIVATE
    glm)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${INCLUDE_DIR})
