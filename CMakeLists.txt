cmake_minimum_required(VERSION 3.16)
project(enpm702_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(src/lecture1)
add_subdirectory(src/lecture2)
add_subdirectory(src/lecture3)
add_subdirectory(src/lecture4)
add_subdirectory(src/lecture5)
add_subdirectory(src/lecture6)
add_subdirectory(src/lecture7)
add_subdirectory(src/lecture8)
add_subdirectory(src/lecture9)
