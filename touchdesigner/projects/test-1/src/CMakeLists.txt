find_package(unofficial-skia CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(touchdesigner CONFIG REQUIRED)

add_library(Canvas STATIC
    Canvas.cpp
)

target_include_directories(Canvas
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Canvas
    PUBLIC
        unofficial::skia::skia
)

add_executable(CanvasTOPTool
    main.cpp
    SdlDrawable.cpp
)

target_link_libraries(CanvasTOPTool
    PRIVATE
        Canvas
        imgui::imgui
        spdlog::spdlog
        SDL3::SDL3
)

add_library(CanvasTOP MODULE
    CanvasTOP.cpp
)

target_link_libraries(CanvasTOP
    PRIVATE
        Canvas
        touchdesigner::Top
)
