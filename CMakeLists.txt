cmake_minimum_required(VERSION 3.16)
project(enpm702_cpp LANGUAGES CXX)

# VS Code's CMake Tools configures this same tree, and its "Unspecified"
# variant passes no CMAKE_BUILD_TYPE. That leaves the cache empty, which means
# no -g (so no breakpoints in the Call Stack exercises) and the cpptools
# warning "the build configurations generated do not contain the active build
# configuration". Default it here so the tree is right however it was
# configured: 702configure, or the editor.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Debug, Release, RelWithDebInfo or MinSizeRel" FORCE)
endif()

# clangd and the VS Code C++ extension read build/compile_commands.json.
# 702configure asks for it; the editor's own configure does not.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic-errors -Wshadow)

# add_subdirectory(project/week1)
# add_subdirectory(project/week2)
# add_subdirectory(project/week3)
# add_subdirectory(project/week4)
add_subdirectory(project/week5)
# add_subdirectory(project/week6)
# add_subdirectory(project/week7)
# add_subdirectory(project/week8)
# add_subdirectory(project/week9)
# add_subdirectory(project/reading_material)
