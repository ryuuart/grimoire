#ifndef CANVAS_H
#define CANVAS_H

#include <SDL3/SDL_gpu.h>
#include <functional>
#include <include/core/SkCanvas.h>
#include <include/core/SkPixmap.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkSurface.h>

class Canvas {
  public:
    Canvas(double width, double height);

    void draw(const std::function<void(SkCanvas *canvas)> &);
    const SkPixmap &getPixmap();

  private:
    sk_sp<SkSurface> m_surface;
    SkPixmap m_pixmap;
};

#endif
