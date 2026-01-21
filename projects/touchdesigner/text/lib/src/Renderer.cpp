#include "Renderer.h"
#include "include/core/SkCanvas.h"
#include <cstddef>
#include <cstdint>

Renderer::Renderer() {
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(1, 1);
    imageInfo = imageInfo.makeColorType(SkColorType::kBGRA_8888_SkColorType);
    surface_ = SkSurfaces::Raster(imageInfo);
}

void Renderer::readPixel(void *buffer) {
    auto image_info = surface_->imageInfo();
    surface_->readPixels(image_info, buffer, image_info.minRowBytes(), 0, 0);
}

size_t Renderer::byteSize() {
    auto image_info = surface_->imageInfo();
    return image_info.computeByteSize(image_info.minRowBytes());
}

void Renderer::resize(uint32_t width, uint32_t height) {
    if (surface_->width() != width || surface_->height() != height)
        surface_ = surface_->makeSurface(width, height);
}

void Renderer::render(const Scene &scene) {
    auto canvas = surface_->getCanvas();

    canvas->clear(SK_ColorTRANSPARENT);

    canvas->drawTextBlob(scene.text.shaper->textBlob, 0, 0,
                         scene.text.style->foregroundColor);
}

uint32_t Renderer::width() { return surface_->width(); }

uint32_t Renderer::height() { return surface_->height(); }
