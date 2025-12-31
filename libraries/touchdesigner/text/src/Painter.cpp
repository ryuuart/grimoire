#include "Painter.h"
#include "Paintable.h"
#include <cstddef>

Painter::Painter(Painter::Description description) {
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(
        description.initialWidth, description.initialHeight);
    imageInfo = imageInfo.makeColorType(SkColorType::kBGRA_8888_SkColorType);
    m_surface = SkSurfaces::Raster(imageInfo);
}
Painter::Painter() : Painter{{}} {}

void Painter::addPaintable(std::shared_ptr<Paintable> paintable) {
    paintables_.emplace(paintable);
}

bool Painter::readPixels(void *buffer) {
    auto image_info = m_surface->imageInfo();
    return m_surface->readPixels(image_info, buffer, image_info.minRowBytes(),
                                 0, 0);
}

void Painter::paint() {
    for (const auto &paintable : paintables_) {
        paintable->paint(m_surface->getCanvas());
    }
}

SkImageInfo Painter::getImageInfo() { return m_surface->imageInfo(); }
