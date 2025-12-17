#include "Canvas.h"
#include "SDL3/SDL_gpu.h"
#include "SdlDrawable.h"
#include "draw.h"
#include "include/core/SkPixmap.h"
#include <cstring>
#include <utility>
#define SDL_MAIN_USE_CALLBACKS
#include "Context.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_video.h"

#define IMGUI_USER_CONFIG "imconfig_overlay.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"
#include "spdlog/spdlog.h"

// Skia
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"

struct AppContext {
    SdlContext sdlContext;
    ToolContext toolContext;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    spdlog::info("Initializing SDL App...");
    SDL_SetAppMetadata("Skia + imgui test proj", "0.1.0", nullptr);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        spdlog::info("Couldn't initialized SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    std::pair<double, double> window_size = {640 * main_scale,
                                             480 * main_scale};
    SDL_WindowFlags window_flags = SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE |
                                   SDL_WINDOW_HIGH_PIXEL_DENSITY |
                                   SDL_WINDOW_HIDDEN;
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
    SDL_GPUDevice *gpu_device =
        SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

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
    ToolContext toolContext{.canvas = Canvas{window_size.first * pixelDensity,
                                             window_size.second * pixelDensity},
                            "",
                            0};

    *appstate = new AppContext{.sdlContext = std::move(sdlContext),
                               .toolContext = std::move(toolContext)};

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppContext *appContext = static_cast<AppContext *>(appstate);
    SdlContext &sdlContext = appContext->sdlContext;
    ToolContext &toolContext = appContext->toolContext;
    SDL_Window *window = sdlContext.window;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true;

    toolContext.totalTime += 20;

    toolContext.canvas.draw(draw, toolContext);

    SkPixmap pixmap = toolContext.canvas.getPixmap();
    SdlDrawable::SdlDrawableDescriptor drawableDescriptor{
        .buffer = pixmap.addr(),
        .sizeInBytes = pixmap.computeByteSize(),
        .width = static_cast<uint32_t>(pixmap.width()),
        .height = static_cast<uint32_t>(pixmap.height()),
    };
    SdlDrawable skiaDrawable(sdlContext.gpuDevice, drawableDescriptor);

    // Start the Dear ImGui frame
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

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

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppContext *appContext = static_cast<AppContext *>(appstate);
    SdlContext &sdlContext = appContext->sdlContext;
    ToolContext &toolContext = appContext->toolContext;
    SDL_Window *window = sdlContext.window;

    switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED: {
        float pixelDensity = SDL_GetWindowPixelDensity(window);
        int width, height;
        width = event->window.data1;
        height = event->window.data2;
        toolContext.canvas.updateSize(width * pixelDensity,
                                      height * pixelDensity);
        break;
    }
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }
    ImGui_ImplSDL3_ProcessEvent(event);

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
