#include "SigilText.h"
#include "Components.h"
#include "Typography.h"
#include <memory>

SigilText::SigilText() {
    content = std::make_shared<ContentComponent>("Architecture Test");
    style = std::make_shared<StyleComponent>();

    Typography &typography = Typography::getInstance();
    style->foregroundColor = SkPaint{SkColors::kBlack};
    auto font_family = typography.getFontManager()->matchFamilyStyle(
        "Helvetica", SkFontStyle());
    style->font = SkFont{font_family, 32};
}

void SigilText::paint(SkCanvas *canvas) {
    canvas->drawString(content->content.c_str(), 10, 10, style->font,
                       style->foregroundColor);
}
