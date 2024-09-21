cmake_minimum_required(VERSION 3.12)
project(
    MySet
)

set(BUILD_EXAMPLE TRUE)
set(BUILD_TESTS TRUE)

add_subdirectory(src)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()