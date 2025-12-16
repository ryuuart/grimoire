#ifndef CONTEXT_H
#define CONTEXT_H
#include "Canvas.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"

struct SdlContext {
    SDL_Window *window;
    SDL_GPUDevice *gpuDevice;
    Uint64 totalTime;
};

struct SkiaContext {
    Canvas canvas{0.0, 0.0};
};

struct AppContext {
    SdlContext sdlContext;
    SkiaContext skiaContext;
};

struct CanvasModel {
    char paragraph[1024 * 5];
};

#endif
