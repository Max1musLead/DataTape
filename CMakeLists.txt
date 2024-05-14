cmake_minimum_required(VERSION 3.15)

project(
    TapeProject
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)
add_subdirectory(lib)
add_subdirectory(parser)

enable_testing()
add_subdirectory(tests)