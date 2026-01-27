#include "Config.h"

#include "Canvas.h"
#include "Context.h"
#include "Gui.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "SdlDrawable.h"
#include "ToolCanvas.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkPixmap.h"
#include "rules_cc/cc/runfiles/runfiles.h"
#include "spdlog/spdlog.h"
#include "tracy/Tracy.hpp"
#include "client/TracyProfiler.hpp"
#include <cstring>
#include <memory>
#include <utility>
#include "Window.h"
#include "Scene.h"
#include "Renderer.h"
#include "TextSystem.h"

using rules_cc::cc::runfiles::Runfiles;

struct AppContext {
    Scene scene;
    std::unique_ptr<TextSystem> textSystem;
    ToolContext toolContext;
    SdlContext sdlContext;
    std::unique_ptr<Runfiles> runfiles;
    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Gui> gui;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    spdlog::info("Initializing SDL App...");
    SDL_SetAppMetadata("Skia + imgui test proj", "0.1.0", nullptr);

    std::string error;
    std::unique_ptr<Runfiles> runfiles(
        Runfiles::Create(argv[0], BAZEL_CURRENT_REPOSITORY, &error));
    if (runfiles == nullptr) {
        spdlog::error("Could not find Bazel runfiles");
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        spdlog::info("Couldn't initialized SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    std::pair<double, double> window_size = {640 * main_scale,
                                             480 * main_scale};
    SDL_WindowFlags window_flags = getWindowFlags();
    SDL_Window *window = SDL_CreateWindow("Testerino", window_size.first,
                                          window_size.second, window_flags);
    if (window == nullptr) {
        spdlog::info("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup GPU Device
    SDL_GPUDevice *gpu_device = makeGpuDevice();
    if (gpu_device == nullptr) {
        spdlog::info("Couldn't create GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Claim window for GPU Device
    if (!SDL_ClaimWindowForGPUDevice(gpu_device, window)) {
        spdlog::info("Couldn't claim window for GPU device: %s",
                     SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetGPUSwapchainParameters(gpu_device, window,
                                  SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                  SDL_GPU_PRESENTMODE_VSYNC);

    // // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    io.Fonts->AddFontFromFileTTF(
        runfiles->Rlocation("imgui/misc/fonts/DroidSans.ttf").c_str());

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLGPU(window);
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = gpu_device;
    init_info.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(gpu_device, window);
    ImGui_ImplSDLGPU3_Init(&init_info);

    float pixelDensity = SDL_GetWindowPixelDensity(window);
    SdlContext sdlContext{.window = window, .gpuDevice = gpu_device};
    ToolContext toolContext{.content = "Test Content",
                            .direction = 0.0,
                            .speed = 0,
                            .totalTime = 0,
                            .offset_xy{0, 0},
                            .fontSize = 32,
                            .contentWidth = 300};

    AppContext *appContext =
        new AppContext{.toolContext = std::move(toolContext),
                       .sdlContext = std::move(sdlContext)};
    appContext->canvas = std::make_unique<ToolCanvas>(
        window_size.first * pixelDensity, window_size.second * pixelDensity,
        appContext->toolContext);
    appContext->gui = std::make_unique<Gui>(appContext->toolContext);
    appContext->textSystem = std::make_unique<TextSystem>(appContext->scene);
    *appstate = appContext;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    if constexpr (TracyIsStarted) {
        ZoneScopedN("Iteration");
    }

    AppContext *appContext = static_cast<AppContext *>(appstate);
    SdlContext &sdlContext = appContext->sdlContext;
    ToolContext &toolContext = appContext->toolContext;
    SDL_Window *window = sdlContext.window;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    toolContext.totalTime = SDL_GetTicks();

    // Start the Dear ImGui frame
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    appContext->gui->draw();

    // appContext->canvas->prepare();
    // appContext->canvas->draw();
    auto& scene = appContext->scene;
    scene.text.content.content = toolContext.content;
    scene.text.style.fontSize = toolContext.fontSize;
    scene.text.style.foregroundColor = {1.0, 1.0, 1.0, 1.0};
    int window_width, window_height;
    SDL_GetWindowSizeInPixels(window, &window_width, &window_height);
    if (window_width <= 0 || window_height <= 0) {
        window_width = 640;
        window_height = 480;
    }
    Renderer renderer{{static_cast<uint32_t>(window_width), static_cast<uint32_t>(window_height), false, true}};
    appContext->textSystem->update(window_width);
    renderer.render(appContext->scene);

    // SkPixmap pixmap = appContext->canvas->getPixmap();
    SkPixmap pixmap = renderer.getPixmap();
    SdlDrawable::SdlDrawableDescriptor drawableDescriptor{
        .buffer = pixmap.addr(),
        .sizeInBytes = pixmap.computeByteSize(),
        .width = static_cast<uint32_t>(pixmap.width()),
        .height = static_cast<uint32_t>(pixmap.height()),
    };
    SdlDrawable skiaDrawable(sdlContext.gpuDevice, drawableDescriptor);

    // Rendering
    ImGui::Render();
    ImDrawData *draw_data = ImGui::GetDrawData();
    const bool is_minimized =
        (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

    SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(
        sdlContext.gpuDevice); // Acquire a GPU command buffer

    SDL_GPUTexture *swapchain_texture;
    SDL_WaitAndAcquireGPUSwapchainTexture(
        command_buffer, window, &swapchain_texture, nullptr,
        nullptr); // Acquire a swapchain texture

    if (swapchain_texture != nullptr && !is_minimized) {
        // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to
        // upload the vertex/index buffer!
        ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);
        skiaDrawable.upload(command_buffer, swapchain_texture);

        // Setup and start a render pass
        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture = swapchain_texture;
        target_info.clear_color = SDL_FColor{clear_color.x, clear_color.y,
                                             clear_color.z, clear_color.w};
        target_info.load_op = SDL_GPU_LOADOP_LOAD;
        target_info.store_op = SDL_GPU_STOREOP_STORE;
        target_info.mip_level = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle = false;
        SDL_GPURenderPass *render_pass =
            SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);
        ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer,
                                         render_pass);
        SDL_EndGPURenderPass(render_pass);
    }

    // Submit the command buffer
    SDL_SubmitGPUCommandBuffer(command_buffer);

    FrameMark;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppContext *appContext = static_cast<AppContext *>(appstate);
    SdlContext &sdlContext = appContext->sdlContext;
    SDL_Window *window = sdlContext.window;

    ImGui_ImplSDL3_ProcessEvent(event);
    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED: {
        float pixelDensity = SDL_GetWindowPixelDensity(window);
        int width, height;
        width = event->window.data1;
        height = event->window.data2;
        appContext->canvas->updateSize(width * pixelDensity,
                                       height * pixelDensity);
        break;
    }
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    spdlog::info("Quitting app...");

    AppContext *appContext = static_cast<AppContext *>(appstate);
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    SDL_WaitForGPUIdle(appContext->sdlContext.gpuDevice);
    SDL_DestroyGPUDevice(appContext->sdlContext.gpuDevice);
    SDL_DestroyWindow(appContext->sdlContext.window);

    delete appContext;
}
