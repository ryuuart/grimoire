#pragma once
#include "Scene.h"
#include "include/core/SkSurface.h"
#include "BufferProvider.h"
#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/BackendTexture.h"

class Renderer {
  public:
    struct RenderConfig {
      uint32_t width;
      uint32_t height;
      bool parallel{false};
      bool useGpu{false};
    };

    explicit Renderer(RenderConfig);
    Renderer(RenderConfig, BufferProvider&);

    uint32_t width() const;
    uint32_t height() const;
    size_t byteSize() const;
    void render(const Scene &scene);
    void readPixel(void *buffer) const;

  private:
    static constexpr SkColorType RENDERER_COLOR_TYPE = SkColorType::kBGRA_8888_SkColorType;

    struct GpuContext {
      std::unique_ptr<skgpu::graphite::Recorder> recorder;
      skgpu::graphite::BackendTexture texture;
      SkPixmap pixmap;
    };

    SkSurfaceProps surface_props_;
    sk_sp<SkSurface> surface_;
    std::unique_ptr<GpuContext> context_{std::make_unique<GpuContext>()};

    bool parallel_;
    bool useGpu_;
};
