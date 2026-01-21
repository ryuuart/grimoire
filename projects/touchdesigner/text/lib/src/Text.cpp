#include "Text.h"
#include "Components.h"
#include "Typography.h"
#include "modules/skshaper/include/SkShaper_harfbuzz.h"
#include "modules/skunicode/include/SkUnicode_icu.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"
#include "modules/skparagraph/include/Paragraph.h"
#include <memory>

using namespace skia::textlayout;

Text::Text() {
    Typography &typography = Typography::getInstance();
    sk_sp<SkUnicode> unicode = SkUnicodes::ICU::Make();

    style = std::make_shared<StyleComponent>();
    style->typeface = SkTypeface::MakeEmpty();
    style->font = SkFont{style->typeface, 24};
    style->textStyle = TextStyle{};
    style->paragraphStyle = ParagraphStyle{};

    content = std::make_shared<TextContentComponent>();
    content->content = "";

    shaper = std::make_shared<TextShaperComponent>();
    shaper->unicode = unicode;
    shaper->shaper = SkShapers::HB::ShapeThenWrap(shaper->unicode,
                                                  typography.getFontManager());
    paragraph = std::make_shared<TextParagraphComponent>();
    paragraph->fontCollection = sk_make_sp<FontCollection>();
    paragraph->builder =  ParagraphBuilder::make(style->paragraphStyle, paragraph->fontCollection, unicode);
    paragraph->paragraph = paragraph->builder->Build();
}
