#include "Text.h"
#include "Components.h"
#include "Typography.h"
#include "modules/skshaper/include/SkShaper_harfbuzz.h"
#include "modules/skunicode/include/SkUnicode_icu.h"
#include <memory>

Text::Text() {
    Typography &typography = Typography::getInstance();

    style = std::make_shared<StyleComponent>();
    style->typeface = SkTypeface::MakeEmpty();
    style->font = SkFont{style->typeface, 24};

    content = std::make_shared<TextContentComponent>();
    content->content = "";

    shaper = std::make_shared<TextShaperComponent>();
    shaper->unicode = SkUnicodes::ICU::Make();
    shaper->shaper = SkShapers::HB::ShapeThenWrap(shaper->unicode,
                                                  typography.getFontManager());
}
