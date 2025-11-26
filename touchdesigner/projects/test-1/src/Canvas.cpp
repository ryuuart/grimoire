#include "Canvas.h"
#include <include/core/SkCanvas.h>
#include <include/core/SkImageInfo.h>
#include <include/core/SkSurface.h>

Canvas::Canvas(double width, double height)
    : m_surface{SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height))} {
}

void Canvas::draw(const std::function<void(SkCanvas *canvas)> &drawFunction) {
    drawFunction(m_surface->getCanvas());
}

const SkPixmap &Canvas::getPixmap() {
    m_surface->peekPixels(&m_pixmap);
    return m_pixmap;
}
