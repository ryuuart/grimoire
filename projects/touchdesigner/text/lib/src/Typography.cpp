#include "Typography.h"
#include "include/core/SkString.h"
#include "FontManager.h"

Typography::Typography() { font_manager_ = makeDefaultFontManager(); }

Typography::FontBook::const_iterator Typography::scanFonts() {
    for (int i = 0; i < font_manager_->countFamilies(); i++) {
        SkString family_name;
        font_manager_->getFamilyName(i, &family_name);

        font_book_.try_emplace(std::string{family_name.c_str()}, nullptr);
    }

    return font_book_.cbegin();
}

void Typography::getFontStyles(
    const std::string &familyName,
    std::function<void(sk_sp<SkFontStyleSet>)> callback) {
    if (font_book_.contains(familyName)) {
        sk_sp<SkFontStyleSet> font_family = font_book_[familyName];

        if (font_family == nullptr) {
            font_family = font_manager_->matchFamily(familyName.c_str());
            font_book_[familyName] = font_family;
        }

        callback(font_family);
    } else {
        callback(nullptr);
    }
}

const sk_sp<SkFontMgr> Typography::getFontManager() const {
    return font_manager_;
}
