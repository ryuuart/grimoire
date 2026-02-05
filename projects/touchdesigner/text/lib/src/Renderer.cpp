#include "Renderer.h"
#include "GpuUtils.h"
#include "include/core/SkPicture.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkCanvas.h"
#include "marl/scheduler.h"
#include "include/gpu/graphite/Surface.h"

using namespace skgpu::graphite;

Renderer::Renderer(const RenderConfig config) {
    gpuContext_ = MakeGpuContext();

    resize(config.initialWidth, config.initialHeight);
    recorder_ = gpuContext_->makeRecorder();
    surface_ = SkSurfaces::RenderTarget(recorder_.get(), imageInfo_);
}

Renderer::~Renderer() {
    while (getStatus() == Status::RENDER_IN_PROGRESS) {}
    gpuContext_->freeGpuResources();
}

void Renderer::resize(const uint32_t width, const uint32_t height) {
    while (getStatus() == Status::RENDER_IN_PROGRESS) {}
    imageInfo_ = SkImageInfo::MakeN32Premul(static_cast<int>(width),
                                            static_cast<int>(height))
            .makeColorType(RENDERER_COLOR_TYPE)
            .makeColorSpace(SkColorSpace::MakeSRGB());
}

void Renderer::startRender(::Buffer &buffer, const Scene &scene) {
    if (getStatus() != Status::RENDER_IN_PROGRESS) {
        surface_ = SkSurfaces::RenderTarget(recorder_.get(), imageInfo_);

        auto canvas = surface_->getCanvas();
        canvas->clipRect(SkRect::MakeIWH(surface_->width(), surface_->height()));
        canvas->clear(0);

        SkPictureRecorder recorder;
        SkCanvas *picture_canvas = recorder.beginRecording(SkRect::MakeIWH(surface_->width(), surface_->height()));
#ifdef USE_SKIA_SHAPED_TEXT
        picture_canvas->drawTextBlob(scene.text.skiaShapedText.textBlob, 0, 0,
                                     scene.text.skiaStyle.foregroundColor);
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
        scene.text.skiaParagraphText.paragraph->paint(picture_canvas, 0, 0);
#endif
        sk_sp<SkPicture> picture = recorder.finishRecordingAsPicture();

        canvas->drawPicture(picture);

        const auto recording = recorder_->snap();
        gpuContext_->insertRecording({recording.get()});
        gpuContext_->asyncRescaleAndReadPixels(surface_.get(), surface_->imageInfo(),
                                               SkIRect::MakeSize(surface_->imageInfo().dimensions()),
                                               SkImage::RescaleGamma::kSrc,
                                               SkImage::RescaleMode::kNearest,
                                               [](void *ctx,
                                                  std::unique_ptr<const SkImage::AsyncReadResult> result) {
                                                   const auto dst_buffer = static_cast<::Buffer *>(ctx);
                                                   if (result && dst_buffer->rowBytes() == result->rowBytes(0)) {
                                                       memcpy(dst_buffer->data(), result->data(0),
                                                              dst_buffer->size());
                                                   }
                                               }, &buffer);
        gpuContext_->submit(SyncToCpu::kNo);
    }
}

Renderer::Status Renderer::getStatus() const {
    gpuContext_->checkAsyncWorkCompletion();
    return gpuContext_->hasUnfinishedGpuWork() ? Status::RENDER_IN_PROGRESS : Status::RENDER_READY;
}

SkImageInfo Renderer::imageInfo() const {
    return imageInfo_;
}
