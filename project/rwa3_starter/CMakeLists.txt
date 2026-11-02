# RWA3 starter. Copy this folder out of the course repository, rename it
# rwa3_firstname_lastname, and build it on its own:
#
#   cmake -S . -B build
#   cmake --build build
#   ./build/rwa3
cmake_minimum_required(VERSION 3.16)
project(rwa3 LANGUAGES CXX)

# Debug by default, so the debugger and Valgrind show file names and lines.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Debug, Release, RelWithDebInfo or MinSizeRel" FORCE)
endif()

# VS Code's C++ extension reads build/compile_commands.json.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Add one line per class: its .cpp file in src/. Its .hpp file goes in include/.
add_executable(rwa3
    src/main.cpp
)
target_include_directories(rwa3 PRIVATE include)
