#ifndef TOOL_CANVAS_H
#define TOOL_CANVAS_H
#include "Canvas.h"
#include "modules/skparagraph/include/ParagraphBuilder.h"

struct ToolContext {
    std::string content;
    uint64_t totalTime;
};

class ToolCanvas : public Canvas {
  public:
    ToolCanvas(double width, double height, ToolContext &);
    virtual ~ToolCanvas() = default;

    void prepare() override;
    void draw() override;

  private:
    ToolContext &m_context;
    std::unique_ptr<skia::textlayout::ParagraphBuilder> m_paragraph_builder;
    skia::textlayout::ParagraphStyle m_paragraph_style;
    sk_sp<skia::textlayout::FontCollection> m_font_collection;
    sk_sp<SkUnicode> m_unicode;
};
#endif
