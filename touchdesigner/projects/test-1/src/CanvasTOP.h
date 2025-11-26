#include "Canvas.h"
#include <CPlusPlus_Common.h>
#include <TOP_CPlusPlusBase.h>

using namespace TD;

#ifndef CANVAS_TOP_H
#define CANVAS_TOP_H

class CanvasTOP : public TOP_CPlusPlusBase {
  public:
    CanvasTOP(const OP_NodeInfo *info, TOP_Context *context);
    virtual ~CanvasTOP();

    virtual void getGeneralInfo(TOP_GeneralInfo *ginfo, const OP_Inputs *inputs,
                                void *reserved1) override;
    virtual void execute(TOP_Output *output, const OP_Inputs *inputs,
                         void *reserved) override;
    virtual void setupParameters(OP_ParameterManager *manager,
                                 void *reserved1) override;

  private:
    Canvas m_canvas;
    TOP_Context *m_context;
    OP_SmartRef<OP_TOPDownloadResult> m_prev_down_res;

    const OP_NodeInfo *m_info;
};

#endif
