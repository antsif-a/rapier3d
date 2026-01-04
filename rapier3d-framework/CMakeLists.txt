cmake_minimum_required(VERSION 4.2.1)
project(rapier3d-framework)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# glfw
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(${EXTERNAL_DIR}/glfw)

add_library(${PROJECT_NAME} 
  ${SOURCES_DIR}/WindowFramework.cpp
  ${SOURCES_DIR}/Window.cpp
  ${SOURCES_DIR}/InputProcessor.cpp
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -O3)

target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_23)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    glfw
  PRIVATE
    rapier3d-gl glm)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${INCLUDE_DIR})
