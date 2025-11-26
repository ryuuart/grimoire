#include "SdlDrawable.h"
#include <SDL3/SDL_gpu.h>
#include <cstring>

SdlDrawable::SdlDrawable(SDL_GPUDevice *fromDevice,
                         const SdlDrawableDescriptor &fromDescriptor)
    : m_device(fromDevice), m_width(fromDescriptor.width),
      m_height(fromDescriptor.height) {
    SDL_GPUTransferBufferCreateInfo createInfo = {};
    createInfo.size = fromDescriptor.sizeInBytes;
    createInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    m_transferBuffer = SDL_CreateGPUTransferBuffer(fromDevice, &createInfo);

    std::memcpy(SDL_MapGPUTransferBuffer(fromDevice, m_transferBuffer, false),
                fromDescriptor.buffer, fromDescriptor.sizeInBytes);
    SDL_UnmapGPUTransferBuffer(fromDevice, m_transferBuffer);
}

SdlDrawable::~SdlDrawable() {
    if (m_transferBuffer) {
        SDL_ReleaseGPUTransferBuffer(m_device, m_transferBuffer);
        m_transferBuffer = nullptr;
    }
}

void SdlDrawable::upload(SDL_GPUCommandBuffer *toBuffer,
                         SDL_GPUTexture *onTexture) {
    SDL_GPUTextureTransferInfo transferInfo = {};
    transferInfo.transfer_buffer = m_transferBuffer;
    transferInfo.pixels_per_row = m_width;
    transferInfo.rows_per_layer = m_height;
    transferInfo.offset = 0;

    SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(toBuffer);
    SDL_GPUTextureRegion destRegion = {};
    destRegion.texture = onTexture;
    destRegion.x = 0;
    destRegion.y = 0;
    destRegion.z = 0;
    destRegion.d = 0;
    destRegion.layer = 0;
    destRegion.mip_level = 0;
    destRegion.w = m_width;
    destRegion.h = m_height;
    SDL_UploadToGPUTexture(copyPass, &transferInfo, &destRegion, true);
    SDL_EndGPUCopyPass(copyPass);
}
