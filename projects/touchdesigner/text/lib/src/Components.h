#pragma once

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include <memory>
#include <string>

#ifdef USE_SKIA_SHAPED_TEXT
#include "modules/skshaper/include/SkShaper.h"
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
#include "modules/skparagraph/include/TextStyle.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#endif

struct ContentComponent {
    std::string content{};
};

struct StyleComponent {
    std::array<float, 4> foregroundColor{0.0f, 0.0f, 0.0f, 0.0f};
    std::string typeFamily{""};
    float fontSize{0.0f};
};

struct SkiaStyleComponent {
    SkPaint foregroundColor;
};

#ifdef USE_SKIA_SHAPED_TEXT
struct SkiaShapedTextComponent {
    struct Style {
        SkFont font;
        sk_sp<SkTypeface> typeface;
    };

    std::unique_ptr<SkShaper> shaper;
    sk_sp<SkTextBlob> textBlob{};
    Style style;
};
#endif

#ifdef USE_SKIA_PARAGRAPH_TEXT
struct SkiaParagraphTextComponent {
    struct Style {
        skia::textlayout::ParagraphStyle paragraphStyle;
    };

    sk_sp<skia::textlayout::FontCollection> fontCollection;
    std::unique_ptr<skia::textlayout::ParagraphBuilder> builder;
    std::unique_ptr<skia::textlayout::Paragraph> paragraph;
    Style style;
};
#endif
