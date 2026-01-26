#include "Renderer.h"

#include "GpuUtils.h"
#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"
#include "glm/vec2.hpp"
#include "include/core/SkCanvas.h"
#include "marl/scheduler.h"
#include "marl/waitgroup.h"
#include "marl/defer.h"
#include "include/gpu/graphite/Surface.h"

using namespace skgpu::graphite;

Renderer::Renderer(const RenderConfig config) : parallel_(config.parallel), useGpu_(config.useGpu) {
    const SkImageInfo image_info = SkImageInfo::MakeN32Premul(static_cast<int>(config.width), static_cast<int>(config.height))
        .makeColorType(RENDERER_COLOR_TYPE)
        .makeColorSpace(SkColorSpace::MakeSRGB());
    if (useGpu_) {
        context_->recorder = MainGpuContext->makeRecorder();
        surface_ = SkSurfaces::RenderTarget(context_->recorder.get(), image_info);
    } else {
        surface_ = SkSurfaces::Raster(image_info);
    }
}

Renderer::Renderer(const RenderConfig config, BufferProvider &buffer_provider) : parallel_(config.parallel), useGpu_(config.useGpu) {
    const SkImageInfo image_info = SkImageInfo::MakeN32Premul(static_cast<int>(config.width), static_cast<int>(config.height))
        .makeColorType(RENDERER_COLOR_TYPE)
        .makeColorSpace(SkColorSpace::MakeSRGB());
    const size_t row_bytes = image_info.minRowBytes();
    const size_t byte_size = image_info.computeByteSize(row_bytes);
    if (useGpu_) {
        surface_props_ = SkSurfaceProps{SkSurfaceProps::kUseDeviceIndependentFonts_Flag, kUnknown_SkPixelGeometry};
        RecorderOptions recorder_options;
        recorder_options.fGpuBudgetInBytes = static_cast<size_t>(512) * 1024 * 1024 * 1024;
        context_->recorder = MainGpuContext->makeRecorder();
        context_->pixmap = SkPixmap{image_info, buffer_provider.provide(byte_size), row_bytes};
        surface_ = SkSurfaces::RenderTarget(context_->recorder.get(), image_info, skgpu::Mipmapped::kNo, &surface_props_);
    } else {
        surface_ = SkSurfaces::WrapPixels(image_info, buffer_provider.provide(byte_size), row_bytes);
    }
}

void Renderer::readPixel(void *buffer) const {
    const auto image_info = surface_->imageInfo();
    surface_->readPixels(image_info, buffer, image_info.minRowBytes(), 0, 0);
}

size_t Renderer::byteSize() const {
    auto image_info = surface_->imageInfo();
    return image_info.computeByteSize(image_info.minRowBytes());
}

void Renderer::render(const Scene &scene) {
    marl::WaitGroup wait_group;
    auto canvas = surface_->getCanvas();
    canvas->clipRect(SkRect::MakeIWH(surface_->width(), surface_->height()));

    if (parallel_ && !useGpu_) {
        wait_group.add(1);
        marl::schedule([=] {
            defer(wait_group.done());
            canvas->clear(0);
        });
    } else {
        canvas->clear(0);
    }

    SkPictureRecorder recorder;
    SkCanvas* picture_canvas = recorder.beginRecording(SkRect::MakeIWH(surface_->width(), surface_->height()));
#ifdef USE_SKIA_SHAPED_TEXT
    picture_canvas->drawTextBlob(scene.text.skiaShapedText.textBlob, 0, 0,
                         scene.text.skiaStyle.foregroundColor);
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
    scene.text.skiaParagraphText.paragraph->paint(picture_canvas, 0, 0);
#endif
    sk_sp<SkPicture> picture = recorder.finishRecordingAsPicture();

    if (parallel_ && !useGpu_) {
        wait_group.wait();

        constexpr uint8_t FACTOR = 4;
        const auto TILE_SIZE = glm::vec2{surface_->width() / FACTOR, surface_->height() / FACTOR};
        constexpr auto TILE_COUNT = FACTOR*FACTOR;
        wait_group.add(TILE_COUNT);
        SkPixmap pixmap;
        surface_->peekPixels(&pixmap);
        for (uint8_t i = 0; i < FACTOR; i++) {
            for (uint8_t j = 0; j < FACTOR; j++) {
                marl::schedule([=] {
                    defer(wait_group.done());
                    const sk_sp<SkSurface> tmp_surface = SkSurfaces::WrapPixels(pixmap.info(), pixmap.writable_addr(), pixmap.rowBytes());
                    auto tmp_canvas = tmp_surface->getCanvas();
                    tmp_canvas->clipRect(SkRect::MakeXYWH(i * TILE_SIZE.x, j * TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y));
                    tmp_canvas->drawPicture(picture);
                });
            }
        }
        wait_group.wait();
    } else {
        canvas->drawPicture(picture);
        if (useGpu_) {
            const auto recording = context_->recorder->snap();
            MainGpuContext->insertRecording({recording.get()});
            MainGpuContext->asyncRescaleAndReadPixels(surface_.get(), surface_->imageInfo(),
                                                         SkIRect::MakeSize(surface_->imageInfo().dimensions()), SkImage::RescaleGamma::kSrc,
                                                         SkImage::RescaleMode::kNearest,
                                                         [](void* ctx,
                                                            std::unique_ptr<const SkImage::AsyncReadResult> result) {
                                                             if (result) {
                                                                 auto output_pixmap = static_cast<SkPixmap*>(ctx);
                                                                 memcpy(output_pixmap->writable_addr(), result->data(0),
                                                                        output_pixmap->computeByteSize());
                                                             }
                                                         }, &context_->pixmap);
            MainGpuContext->submit(SyncToCpu::kYes);
        }
    }
}

uint32_t Renderer::width() const { return surface_->width(); }

uint32_t Renderer::height() const { return surface_->height(); }
