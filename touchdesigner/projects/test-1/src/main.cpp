#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_metal.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_properties.h>
#include <algorithm>
#include <cstring>
#include <include/core/SkMatrix.h>
#include <include/core/SkPixmap.h>
#include <include/private/base/SkPoint_impl.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#define IMGUI_USER_CONFIG "imconfig_overlay.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
#include <imgui_impl_metal.h>
#include <spdlog/spdlog.h>

// Skia
#include "include/core/SkImage.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkCanvas.h"

static SDL_Window *window = nullptr;

struct SdlContext {
    SDL_Window* window;
    SDL_GPUDevice* gpuDevice;
};

struct SkiaContext {
    sk_sp<SkSurface> surface;
};

struct AppContext {
    SdlContext sdlContext;
    SkiaContext skiaContext;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    spdlog::info("Initializing SDL App...");
    SDL_SetAppMetadata("Skia + imgui test proj", "0.1.0", nullptr);

    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        spdlog::info("Couldn't initialized SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    std::pair<double, double> window_size = {640 * main_scale, 480 * main_scale};
    SDL_WindowFlags window_flags = SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN;
    window = SDL_CreateWindow("Testerino", window_size.first, window_size.second, window_flags);
    if(window == nullptr) {
        spdlog::info("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup GPU Device
    SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
    if (gpu_device == nullptr) {
        spdlog::info("Couldn't create GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Claim window for GPU Device
    if(!SDL_ClaimWindowForGPUDevice(gpu_device, window)) {
        spdlog::info("Couldn't claim window for GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetGPUSwapchainParameters(gpu_device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLGPU(window);
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = gpu_device;
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu_device, window);
    ImGui_ImplSDLGPU3_Init(&init_info);

    SkiaContext skiaContext {
        .surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(window_size.first, window_size.second))
    };

    *appstate = new AppContext {
        .sdlContext = SdlContext {
            .window = window,
            .gpuDevice = gpu_device
        },
        .skiaContext = skiaContext
    };

    return SDL_APP_CONTINUE;
}

SDL_AppResult  SDL_AppIterate(void *appstate) {
    AppContext* appContext = static_cast<AppContext*>(appstate);
    SdlContext& sdlContext = appContext->sdlContext;
    SkiaContext& skiaContext = appContext->skiaContext;
    SkCanvas* canvas = skiaContext.surface->getCanvas();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true;

    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPathBuilder path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < 7; ++i) {
        SkScalar theta = 3 * i * TAU / 7;
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    canvas->clear(SK_ColorBLUE);
    path.transform(SkMatrix::Translate(SkVector{0.5f * scale, 0.5f * scale}));
    canvas->drawPath(path.detach(), p);
    SkPixmap pixmap;
    skiaContext.surface->peekPixels(&pixmap);

    SDL_GPUTransferBufferCreateInfo create_info = {};
    create_info.size = pixmap.computeByteSize();
    create_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* skiaTransferBuffer = SDL_CreateGPUTransferBuffer(sdlContext.gpuDevice, &create_info);
    void* sdlSkiaTransferBufferAddr = SDL_MapGPUTransferBuffer(sdlContext.gpuDevice, skiaTransferBuffer, false);
    memcpy(sdlSkiaTransferBufferAddr, pixmap.addr(), pixmap.computeByteSize());
    SDL_UnmapGPUTransferBuffer(sdlContext.gpuDevice, skiaTransferBuffer);
    SDL_GPUTextureTransferInfo sdlTransferInfo = {};
    sdlTransferInfo.transfer_buffer = skiaTransferBuffer;
    sdlTransferInfo.offset = 0;
    sdlTransferInfo.rows_per_layer = pixmap.height();
    sdlTransferInfo.pixels_per_row = pixmap.width();

    // Start the Dear ImGui frame
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(sdlContext.gpuDevice); // Acquire a GPU command buffer

    SDL_GPUTexture* swapchain_texture;
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

    if (swapchain_texture != nullptr && !is_minimized)
    {
        // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
        Imgui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

        auto copyPass = SDL_BeginGPUCopyPass(command_buffer);
        SDL_GPUTextureRegion dest_region = {};
        dest_region.texture = swapchain_texture;
        dest_region.x = 0;
        dest_region.y = 0;
        dest_region.z = 0;
        dest_region.d = 0;
        dest_region.layer = 0;
        dest_region.mip_level = 0;
        dest_region.w = pixmap.width();
        dest_region.h = pixmap.height();
        SDL_UploadToGPUTexture(copyPass, &sdlTransferInfo, &dest_region, true);
        SDL_EndGPUCopyPass(copyPass);

        // Setup and start a render pass
        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture = swapchain_texture;
        target_info.clear_color = SDL_FColor { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
        target_info.load_op = SDL_GPU_LOADOP_LOAD;
        target_info.store_op = SDL_GPU_STOREOP_STORE;
        target_info.mip_level = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle = false;
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);
        ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);
        SDL_EndGPURenderPass(render_pass);
    }

    // Submit the command buffer
    SDL_SubmitGPUCommandBuffer(command_buffer);

    SDL_ReleaseGPUTransferBuffer(sdlContext.gpuDevice, skiaTransferBuffer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    ImGui_ImplSDL3_ProcessEvent(event);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    spdlog::info("Quitting app...");

    AppContext* appContext = static_cast<AppContext*>(appstate);
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    SDL_WaitForGPUIdle(appContext->sdlContext.gpuDevice);
    SDL_DestroyGPUDevice(appContext->sdlContext.gpuDevice);
    SDL_DestroyWindow(appContext->sdlContext.window);

    delete appContext;
}
