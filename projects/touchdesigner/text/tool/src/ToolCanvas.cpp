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
#include <cmath>
#include <cstdint>
#include <tracy/TracyC.h>
#include <tracy/Tracy.hpp>
#include "include/core/SkTypes.h"

using namespace skia::textlayout;
using namespace choreograph;
using namespace glm;

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
    SkPaint p;
    p.setAntiAlias(true);
    m_canvas->clear(SK_ColorWHITE);

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
    paragraph->layout(std::numeric_limits<float>::infinity());
    paragraph->layout(std::max(paragraph->getMinIntrinsicWidth() + 1, static_cast<SkScalar>(m_context.contentWidth)));

    //
    // draw paragraphs
    //

    Time time = m_context.totalTime * 0.001;

    uvec2 contentSize{paragraph->getLongestLine(), paragraph->getHeight()};
    uvec2 canvasSize{m_canvas->getSurface()->width(),
                     m_canvas->getSurface()->height()};
    uvec2 clones{std::ceil(canvasSize.x / contentSize.x),
                 std::ceil(canvasSize.y / contentSize.y)};
    ivec2 translation = dvec2{std::cos(radians(m_context.direction)),
                              -std::sin(radians(m_context.direction))} *
                        static_cast<double>(m_context.speed) * time;
    ivec2 startOffset = {
        (translation.x + m_context.offset_xy[0]) % contentSize.x,
        (translation.y + m_context.offset_xy[1]) % contentSize.y};

    if (contentSize.x >= 1 && contentSize.y >= 1) {
        for (int64_t i = -static_cast<int64_t>(contentSize.x) + startOffset.x;
             i < canvasSize.x; i += contentSize.x) {
            for (int64_t j =
                     -static_cast<int64_t>(contentSize.y) + startOffset.y;
                 j < canvasSize.y; j += contentSize.y) {
                paragraph->paint(m_canvas, i, j);
            }
        }
    }
}
