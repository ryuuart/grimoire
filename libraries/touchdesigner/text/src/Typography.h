#pragma once
#include "absl/container/flat_hash_map.h"
#include "include/core/SkFontMgr.h"
#include <string>

class Typography {
  public:
    using FontBook = absl::flat_hash_map<std::string, sk_sp<SkFontStyleSet>>;

    static Typography &getInstance() {
        static Typography instance;

        return instance;
    }

    FontBook::const_iterator scanFonts();
    void getFontStyles(const std::string &familyName,
                       std::function<void(sk_sp<SkFontStyleSet>)> callback);

    const sk_sp<SkFontMgr> getFontManager() const;

  private:
    Typography();
    Typography(const Typography &) = delete;
    void operator=(const Typography &) = delete;

    sk_sp<SkFontMgr> font_manager_;
    FontBook font_book_;
};
