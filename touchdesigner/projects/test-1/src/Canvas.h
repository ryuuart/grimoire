#ifndef CANVAS_H
#define CANVAS_H

#include <functional>
#include <include/core/SkCanvas.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkPixmap.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkSurface.h>
#include <include/ports/SkFontMgr_mac_ct.h>

class Canvas {
  public:
    Canvas(double width, double height);

    void draw(const std::function<void(SkCanvas *canvas)> &);
    void updateSize(int width, int height);
    const SkPixmap &getPixmap();
    const sk_sp<SkFontMgr> getFontManager();

  private:
    sk_sp<SkSurface> m_surface;
    sk_sp<SkFontMgr> m_fontManager;
    SkPixmap m_pixmap;
};

#endif
