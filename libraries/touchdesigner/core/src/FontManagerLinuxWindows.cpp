#include "FontManager.h"

#include "include/ports/SkFontMgr_fontconfig.h"
#include "include/ports/SkFontScanner_FreeType.h"

sk_sp<SkFontMgr> makeDefaultFontManager() {
     return SkFontMgr_New_FontConfig(nullptr, SkFontScanner_Make_FreeType());
}