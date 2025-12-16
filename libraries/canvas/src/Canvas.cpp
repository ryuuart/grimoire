#include "Canvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorType.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/ports/SkFontMgr_mac_ct.h"
#include <cstddef>

Canvas::Canvas(double width, double height) {
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(width, height);
    imageInfo = imageInfo.makeColorType(SkColorType::kBGRA_8888_SkColorType);
    m_surface = SkSurfaces::Raster(imageInfo);

    SkFontMgr_New_CoreText(nullptr);
    m_fontManager = SkFontMgr_New_CoreText(nullptr);
}

void Canvas::updateSize(int width, int height) {
    if (m_surface->width() != width || m_surface->height() != height) {
        m_surface = m_surface->makeSurface(width, height);
    }
}

const SkPixmap &Canvas::getPixmap() {
    m_surface->peekPixels(&m_pixmap);
    return m_pixmap;
}

const sk_sp<SkFontMgr> Canvas::getFontManager() { return m_fontManager; }
