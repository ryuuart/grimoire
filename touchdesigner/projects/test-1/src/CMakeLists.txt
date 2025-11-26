find_package(unofficial-skia CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(touchdesigner CONFIG REQUIRED)

qt_add_executable(Test
    main.cpp
    Canvas.cpp
    SdlDrawable.cpp
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
        touchdesigner::Top
)

target_compile_options(Test
    PRIVATE
        -objc-arc
)
