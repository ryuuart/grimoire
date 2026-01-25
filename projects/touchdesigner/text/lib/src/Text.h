#pragma once

#include "Components.h"
#include <memory>

struct Text {
    ContentComponent content;
    StyleComponent style;
    SkiaStyleComponent skiaStyle;
#ifdef USE_SKIA_SHAPED_TEXT
    SkiaShapedTextComponent skiaShapedText;
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
    SkiaParagraphTextComponent skiaParagraphText;
#endif
};
