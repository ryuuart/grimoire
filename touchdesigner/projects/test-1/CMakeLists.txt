cmake_minimum_required(VERSION 3.31)
project(touchdesigner_canvas LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory(src)
