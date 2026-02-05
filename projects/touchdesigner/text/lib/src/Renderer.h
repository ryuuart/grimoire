#pragma once
#include "Buffer.h"
#include "Scene.h"
#include "include/core/SkSurface.h"
#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/BackendTexture.h"
#include "include/core/SkBitmap.h"

class Renderer {
  public:
    struct RenderConfig {
      uint32_t initialWidth;
      uint32_t initialHeight;
    };

    enum class Status {
      RENDER_IN_PROGRESS,
      RENDER_READY,
    };

    Renderer(RenderConfig config);
    ~Renderer();

    SkImageInfo imageInfo() const;
    Status getStatus() const;
    void startRender(::Buffer& buffer, const Scene &scene);
    void resize(uint32_t width, uint32_t height);

  private:
    static constexpr SkColorType RENDERER_COLOR_TYPE = kBGRA_8888_SkColorType;

    SkSurfaceProps surfaceProps_;
    sk_sp<SkSurface> surface_;
    SkImageInfo imageInfo_;
    std::unique_ptr<skgpu::graphite::Context> gpuContext_;
    std::unique_ptr<skgpu::graphite::Recorder> recorder_;
};
