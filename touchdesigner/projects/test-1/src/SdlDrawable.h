#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL3/SDL_gpu.h>
#include <cstdint>
class SdlDrawable {
  public:
    struct SdlDrawableDescriptor {
        const void *buffer;
        uint64_t sizeInBytes;
        uint32_t width;
        uint32_t height;
    };
    SdlDrawable(SDL_GPUDevice *fromDevice,
                const SdlDrawableDescriptor &fromDescriptor);
    virtual ~SdlDrawable();

    void upload(SDL_GPUCommandBuffer *toBuffer, SDL_GPUTexture *onTexture);

  private:
    SDL_GPUTransferBuffer *m_transferBuffer;
    SDL_GPUDevice *m_device;

    uint32_t m_width;
    uint32_t m_height;
};

#endif
