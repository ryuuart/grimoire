#include "Canvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorType.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "FontManager.h"
#include <cstddef>

Canvas::Canvas(double width, double height) {
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(width, height);
    imageInfo = imageInfo.makeColorType(SkColorType::kBGRA_8888_SkColorType);
    m_surface = SkSurfaces::Raster(imageInfo);

    m_canvas = m_surface->getCanvas();

    m_fontManager = makeDefaultFontManager();

    m_canvas->clipRect(SkRect::MakeIWH(m_surface->width(), m_surface->height()));
}

Canvas::~Canvas() { m_canvas = nullptr; }

void Canvas::updateSize(int width, int height) {
    if (m_surface->width() != width || m_surface->height() != height) {
        m_surface = m_surface->makeSurface(width, height);
        m_canvas = m_surface->getCanvas();
        m_canvas->clipRect(SkRect::MakeIWH(m_surface->width(), m_surface->height()));
    }
}

const SkPixmap &Canvas::getPixmap() {
    m_surface->peekPixels(&m_pixmap);
    return m_pixmap;
}

const sk_sp<SkFontMgr> Canvas::getFontManager() { return m_fontManager; }
