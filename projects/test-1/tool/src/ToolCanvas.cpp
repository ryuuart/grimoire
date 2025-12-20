#include "ToolCanvas.h"
#include "Canvas.h"
#include "choreograph/Choreograph.h"
#include "glm/glm.hpp"
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkMatrix.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPathBuilder.h"
#include "modules/skunicode/include/SkUnicode_icu.h"
#include <cstdint>

using namespace skia::textlayout;
using namespace choreograph;
using vec2 = glm::vec2;

ToolCanvas::ToolCanvas(double width, double height, ToolContext &context)
    : Canvas{width, height}, m_context{context},
      m_font_collection{sk_make_sp<FontCollection>()},
      m_unicode{SkUnicodes::ICU::Make()} {
    m_font_collection->setAssetFontManager(m_fontManager);
    m_paragraph_builder =
        ParagraphBuilder::make(m_paragraph_style, m_font_collection, m_unicode);
}

void ToolCanvas::prepare() { m_paragraph_builder->Reset(); }

void ToolCanvas::draw() {
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
    m_canvas->clear(SK_ColorBLUE);
    path.transform(SkMatrix::RotateDeg(m_context.totalTime * 0.01));
    path.transform(SkMatrix::Translate(SkVector{0.5f * scale, 0.5f * scale}));
    auto finalPath = path.detach();
    m_canvas->drawPath(path.detach(), p);

    m_canvas->setMatrix(
        SkMatrix::Translate(m_context.offset_xy[0], m_context.offset_xy[1]));

    sk_sp<SkTypeface> typeface =
        m_fontManager->matchFamilyStyle("Akzidenz-Grotesk Next", SkFontStyle());
    SkFont font{typeface, 32};

    // paragraph logic
    TextStyle textStyle;
    textStyle.setFontSize(m_context.fontSize);
    textStyle.setForegroundPaint(p);
    textStyle.setBackgroundPaint(SkPaint{SkColor4f{1, 1, 1, 1}});
    m_paragraph_builder->pushStyle(textStyle);
    m_paragraph_builder->addText(m_context.content.c_str());
    std::unique_ptr<Paragraph> paragraph = m_paragraph_builder->Build();
    paragraph->layout(m_context.contentWidth);

    auto translation = makeRamp(vec2(0, 0), vec2(m_context.translation, 0),
                                m_context.duration, EaseNone());

    // draw paragraphs
    for (uint8_t i = 0; i < m_context.clones_xy[0]; i++) {
        for (uint8_t j = 0; j < m_context.clones_xy[1]; j++) {
            paragraph->paint(
                m_canvas, i * paragraph->getLongestLine(),
                j * paragraph->getHeight() +
                    translation
                        ->getValue(wrapTime(m_context.totalTime * 0.001,
                                            m_context.duration))
                        .x);
        }
    }
}
