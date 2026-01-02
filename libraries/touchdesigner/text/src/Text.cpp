#include "Text.h"
#include "Components.h"
#include "Typography.h"
#include <memory>

Text::Text() {
    content = std::make_shared<TextContentComponent>("Architecture Test");
    style = std::make_shared<StyleComponent>();

    Typography &typography = Typography::getInstance();
    style->foregroundColor = SkPaint{SkColors::kBlack};
    auto font_family = typography.getFontManager()->matchFamilyStyle(
        "Helvetica", SkFontStyle());
    style->font = SkFont{font_family, 32};
}
