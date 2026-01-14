#pragma once

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_gpu.h"

extern const SDL_WindowFlags SHARED_WINDOW_FLAGS;

SDL_WindowFlags getWindowFlags();

SDL_GPUDevice* makeGpuDevice();