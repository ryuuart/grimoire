#include "Window.h"

SDL_WindowFlags getWindowFlags() {
    return SDL_WINDOW_METAL | SHARED_WINDOW_FLAGS;
}

SDL_GPUDevice* makeGpuDevice() {
    return SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_METALLIB, true, nullptr);
}