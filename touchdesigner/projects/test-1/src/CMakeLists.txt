find_package(unofficial-skia CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

qt_add_executable(Test
    main.cpp
)

target_include_directories(Test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Test
    PRIVATE
        unofficial::skia::skia
        Qt6::Core
        imgui::imgui
        spdlog::spdlog
        SDL3::SDL3
)

target_compile_options(Test
    PRIVATE
        -objc-arc
)
