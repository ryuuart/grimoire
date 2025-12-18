#ifndef TOOL_CANVAS_H
#define TOOL_CANVAS_H
#include "Canvas.h"

struct ToolContext {
    std::string content;
    uint64_t totalTime;
};

class ToolCanvas : public Canvas {
  public:
    ToolCanvas(double width, double height, ToolContext &);
    virtual ~ToolCanvas() = default;
    void draw() override;

  private:
    ToolContext &m_context;
};
#endif
