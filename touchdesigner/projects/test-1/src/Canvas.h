#ifndef CANVAS_H
#define CANVAS_H

#include <functional>
#include <include/core/SkCanvas.h>
#include <include/core/SkPixmap.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkSurface.h>

class Canvas {
  public:
    Canvas(double width, double height);

    void draw(const std::function<void(SkCanvas *canvas)> &);
    void updateSize(int width, int height);
    const SkPixmap &getPixmap();

  private:
    sk_sp<SkSurface> m_surface;
    SkPixmap m_pixmap;
};

#endif
