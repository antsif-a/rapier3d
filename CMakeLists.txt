cmake_minimum_required(VERSION 3.6)
project(r3d)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# global dependencies
add_subdirectory(external/glm)

# rapier3d modules
add_subdirectory(rapier3d-gl)
add_subdirectory(rapier3d-framework)
add_subdirectory(rapier3d)

add_subdirectory(game)