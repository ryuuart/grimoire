#pragma once

#include "Paintable.h"
#include "absl/container/flat_hash_set.h"
#include "include/core/SkSurface.h"
#include <cstddef>
#include <cstdint>
#include <memory>

class Painter {
  public:
    enum class BackendType { CPU, GPU };

    struct Description {
        uint32_t initialWidth{256};
        uint32_t initialHeight{256};
        BackendType backendType{BackendType::GPU};
    };

    Painter();
    Painter(Description description);

    bool readPixels(void *buffer);
    SkImageInfo getImageInfo();
    void paint();
    void resize(uint32_t width, uint32_t height);
    void addPaintable(std::shared_ptr<Paintable> paintable);
    void removePaintable(std::shared_ptr<Paintable> paintable);

  private:
    sk_sp<SkSurface> m_surface;

    absl::flat_hash_set<std::shared_ptr<Paintable>> paintables_;
};
