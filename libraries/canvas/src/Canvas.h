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

    virtual void prepare() = 0;
    virtual void draw() = 0;

    void updateSize(int width, int height);
    const SkPixmap &getPixmap();
    const sk_sp<SkFontMgr> getFontManager();

  protected:
    sk_sp<SkFontMgr> m_fontManager;
    SkCanvas *m_canvas;

  private:
    sk_sp<SkSurface> m_surface;
    SkPixmap m_pixmap;
};

#endif
