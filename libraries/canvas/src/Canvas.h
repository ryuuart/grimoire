#ifndef CANVAS_H
#define CANVAS_H

#include "include/core/SkCanvas.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/ports/SkFontMgr_mac_ct.h"

class Canvas {
  public:
    Canvas(double width, double height);

    template <typename F, typename C>
        requires std::invocable<F &, SkCanvas *, C &>
    void draw(F &&drawFunction, C &&context) {
        drawFunction(m_surface->getCanvas(), context);
    }

    void updateSize(int width, int height);
    const SkPixmap &getPixmap();
    const sk_sp<SkFontMgr> getFontManager();

  private:
    sk_sp<SkSurface> m_surface;
    sk_sp<SkFontMgr> m_fontManager;
    SkPixmap m_pixmap;
};

#endif
