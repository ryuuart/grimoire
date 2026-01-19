#pragma once
#include "Scene.h"
#include "include/core/SkSurface.h"
#include <cstddef>
#include <cstdint>

class Renderer {
  public:
    Renderer();

    uint32_t width();
    uint32_t height();
    size_t byteSize();
    void render(const Scene &scene);
    void readPixel(void *buffer);
    void resize(uint32_t width, uint32_t height);

  private:
    sk_sp<SkSurface> surface_;
};
