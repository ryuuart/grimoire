#ifndef DRAW_H
#define DRAW_H
#include "Canvas.h"
#include "include/core/SkCanvas.h"

struct ToolContext {
    Canvas canvas;
    char paragraph[1024 * 5];
    uint64_t totalTime;
};

void draw(SkCanvas *canvas, ToolContext &context);

#endif
