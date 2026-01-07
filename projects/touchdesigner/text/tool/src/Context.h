#ifndef CONTEXT_H
#define CONTEXT_H
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"

struct SdlContext {
    SDL_Window *window;
    SDL_GPUDevice *gpuDevice;
};

#endif
