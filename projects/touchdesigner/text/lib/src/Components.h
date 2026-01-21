#pragma once

#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "modules/skshaper/include/SkShaper.h"
#include "modules/skunicode/include/SkUnicode.h"
#include "modules/skparagraph/include/TextStyle.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include <cstdint>
#include <memory>
#include <string>

struct TransformComponent {
    uint64_t x;
    uint64_t y;
};

struct SizeComponent {
    uint64_t width;
    uint64_t height;
};

struct TextContentComponent {
    std::string content{};
};

struct TextShaperComponent {
    std::unique_ptr<SkShaper> shaper;
    sk_sp<SkUnicode> unicode;
    sk_sp<SkTextBlob> textBlob{};
    std::unique_ptr<SkShaper::FontRunIterator> font;
    std::unique_ptr<SkShaper::ScriptRunIterator> script;
    std::unique_ptr<SkShaper::BiDiRunIterator> bidi;
    std::unique_ptr<SkShaper::LanguageRunIterator> language;
};

struct StyleComponent {
    SkPaint foregroundColor;
    SkFont font;
    sk_sp<SkTypeface> typeface;
    skia::textlayout::ParagraphStyle paragraphStyle;
    skia::textlayout::TextStyle textStyle;
};

struct TextParagraphComponent {
    sk_sp<SkUnicode> unicode;
    sk_sp<skia::textlayout::FontCollection> fontCollection;
    std::unique_ptr<skia::textlayout::Paragraph> paragraph;
    std::unique_ptr<skia::textlayout::ParagraphBuilder> builder;
};
