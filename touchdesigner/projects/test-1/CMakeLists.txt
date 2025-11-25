cmake_minimum_required(VERSION 3.31)
project(touchdesigner_test_1)

list(APPEND CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")
find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_subdirectory(src)
