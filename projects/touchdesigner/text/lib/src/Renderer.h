#pragma once
#include "Scene.h"
#include "include/core/SkSurface.h"

#include "BufferProvider.h"

class Renderer {
  public:
    struct RenderConfig {
      uint32_t width;
      uint32_t height;
      bool parallel{false};
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

    sk_sp<SkSurface> surface_;

    bool parallel_;
};
