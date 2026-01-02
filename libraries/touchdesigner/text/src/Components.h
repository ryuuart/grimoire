#pragma once

#include "include/core/SkFont.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkPaint.h"
#include <cstdint>
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
    std::string content;
};

struct StyleComponent {
    SkFont font;
    SkFontStyle style;
    SkPaint foregroundColor;
};
