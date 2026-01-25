//
// Created by Long Nguyen on 1/23/26.
//

#include "TextSystem.h"
#include "modules/skunicode/include/SkUnicode_icu.h"

#ifdef USE_SKIA_SHAPED_TEXT
#include "modules/skshaper/include/SkShaper_harfbuzz.h"
#include "modules/skshaper/include/SkShaper_skunicode.h"
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
using namespace skia::textlayout;
#endif

TextSystem::TextSystem(Scene &scene) : scene_(scene), typography_(Typography::getInstance()),
                                       unicode_(SkUnicodes::ICU::Make()) {
    Text& text = scene_.text;
    text.content.content = "";
    text.style.foregroundColor.fill(0.0f);
    text.style.typeFamily = "System";
    text.style.fontSize = 32;
#ifdef USE_SKIA_SHAPED_TEXT
    SkiaShapedTextComponent& skia_shaped_text = text.skiaShapedText;
    skia_shaped_text.shaper = SkShapers::HB::ShapeThenWrap(unicode_, typography_.getFontManager());
    skia_shaped_text.style.typeface = typography_.getFontManager()->matchFamilyStyle("Helvetica", SkFontStyle{}); // TODO: create method to pull in Noto Sans as a default typeface
    skia_shaped_text.style.font = SkFont{skia_shaped_text.style.typeface, text.style.fontSize};
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
    SkiaParagraphTextComponent& skia_paragraph_text = text.skiaParagraphText;
    skia_paragraph_text.style.paragraphStyle = ParagraphStyle{};
    TextStyle text_style;
    skia_paragraph_text.style.paragraphStyle.setTextStyle(text_style);
    skia_paragraph_text.fontCollection = sk_make_sp<FontCollection>();
    skia_paragraph_text.fontCollection->setDefaultFontManager(typography_.getFontManager());
    skia_paragraph_text.builder = ParagraphBuilder::make(skia_paragraph_text.style.paragraphStyle, skia_paragraph_text.fontCollection, unicode_);
#endif
}

void TextSystem::update(const float width) const {
    Text& text = scene_.text;
    const std::string &content = text.content.content;

    auto& [foregroundColor] = text.skiaStyle;
    foregroundColor = SkPaint{SkColor4f{
        text.style.foregroundColor[0],
        text.style.foregroundColor[1],
        text.style.foregroundColor[2],
        text.style.foregroundColor[3]
    }};
    const float fontSize  = text.style.fontSize;

#ifdef USE_SKIA_SHAPED_TEXT
    const std::size_t size = content.size();
    text.skiaShapedText.style.font.setSize(fontSize);

    const auto font_iterator = SkShaper::MakeFontMgrRunIterator(
        content.c_str(), size, text.skiaShapedText.style.font, typography_.getFontManager());
    const auto bidi_iterator = SkShapers::unicode::BidiRunIterator(unicode_, content.c_str(), size, SkBidiIterator::kLTR);
    const auto langauge_iterator = SkShaper::MakeStdLanguageRunIterator(content.c_str(), size);
    const auto script_iterator= SkShapers::HB::ScriptRunIterator(content.c_str(), size);
    auto run_handler = SkTextBlobBuilderRunHandler(text.content.content.c_str(), {0, 0});
    text.skiaShapedText.shaper->shape(content.c_str(), size, *font_iterator, *bidi_iterator, *script_iterator,
                                      *langauge_iterator, nullptr, 0, width, &run_handler);
    text.skiaShapedText.textBlob = run_handler.makeBlob();
#endif
#ifdef USE_SKIA_PARAGRAPH_TEXT
    SkiaParagraphTextComponent& paragraph_text = text.skiaParagraphText;
    paragraph_text.builder->Reset();
    TextStyle text_style;
    text_style.setFontFamilies({SkString{text.style.typeFamily}});
    text_style.setForegroundColor(foregroundColor);
    text_style.setFontSize(fontSize);
    paragraph_text.builder->pushStyle(text_style);
    paragraph_text.builder->addText(content.c_str());
    paragraph_text.builder->pop();
    paragraph_text.paragraph = text.skiaParagraphText.builder->Build();
    paragraph_text.paragraph->layout(width);
#endif
}

