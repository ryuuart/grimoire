#ifndef GUI_H
#define GUI_H

#include "ToolCanvas.h"
class Gui {
  public:
    Gui(ToolContext &context);

    void draw();

  private:
    ToolContext &m_context;
};

#endif
