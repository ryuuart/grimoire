#include "FontManager.h"

#include "include/ports/SkFontMgr_mac_ct.h"

sk_sp<SkFontMgr> makeDefaultFontManager() {
     return SkFontMgr_New_CoreText(nullptr);
}
