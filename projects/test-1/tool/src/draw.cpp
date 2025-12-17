#include "draw.h"
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPathBuilder.h"

void draw(SkCanvas *canvas, ToolContext &context) {
    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPathBuilder path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < 7; ++i) {
        SkScalar theta = 3 * i * TAU / 7;
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    canvas->clear(SK_ColorBLUE);
    path.transform(SkMatrix::RotateDeg(context.totalTime * 0.01));
    path.transform(SkMatrix::Translate(SkVector{0.5f * scale, 0.5f * scale}));
    auto finalPath = path.detach();
    for (int i = 0; i < 5; i++) {
        canvas->drawPath(finalPath, p);
        finalPath.transform(SkMatrix::Translate(i * 100, i * 100));
    }
    canvas->drawPath(path.detach(), p);

    sk_sp<SkTypeface> typeface =
        context.canvas.getFontManager()->matchFamilyStyle(
            "Akzidenz-Grotesk Next", SkFontStyle());
    SkFont font{typeface, 32};
    for (int i = 0; i < 10; i++) {
        canvas->drawString((std::string{"Testerino! "} +
                            std::to_string(context.totalTime * 0.5 * i))
                               .c_str(),
                           10, (i + 1) * 20, font, p);
    }
    canvas->translate(cos(context.totalTime * 0.01) * 10,
                      sin(context.totalTime * 0.01) * 10);
}
