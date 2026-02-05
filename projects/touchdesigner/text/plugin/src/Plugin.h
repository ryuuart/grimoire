#pragma once
#include "Parameters.h"
#include "Renderer.h"
#include "Scene.h"
#include "TDBuffer.h"
#include "TextSystem.h"
#include "TOP_CPlusPlusBase.h"
#include "external/skia+/src/gpu/graphite/Renderer.h"

class SigilTextPlugin : public TD::TOP_CPlusPlusBase {
  public:
    SigilTextPlugin(const TD::OP_NodeInfo *info, TD::TOP_Context *context);
    virtual ~SigilTextPlugin();

    virtual void getGeneralInfo(TD::TOP_GeneralInfo *ginfo,
                                const TD::OP_Inputs *inputs,
                                void *reserved1) override;
    virtual void execute(TD::TOP_Output *output, const TD::OP_Inputs *inputs,
                         void *reserved) override;
    virtual void setupParameters(TD::OP_ParameterManager *manager,
                                 void *reserved1) override;

  private:
    TD::TOP_Context *m_context;
    TD::OP_SmartRef<TD::OP_TOPDownloadResult> m_prev_down_res;

    const TD::OP_NodeInfo *m_info;
    Parameters parameters_;

    std::unique_ptr<TDBuffer> backBuffer_;
    std::unique_ptr<TDBuffer> frontBuffer_;

    Scene scene_;
    Renderer renderer_{{1, 1}};
    std::unique_ptr<TextSystem> textSystem_;
};
