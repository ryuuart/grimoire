#ifndef DRAW_H
#define DRAW_H
#include "Context.h"
#include "include/core/SkCanvas.h"

struct CanvasContext {
    SdlContext &sdlContext;
    SkiaContext &skiaContext;
};

void draw(SkCanvas *canvas, CanvasContext &context);

#endif
