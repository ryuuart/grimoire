#include "Plugin.h"
#include "CPlusPlus_Common.h"
#include "Renderer.h"
#include "TOP_CPlusPlusBase.h"
#include "Typography.h"
#include "TDBuffer.h"
#include "external/+_repo_rules+marl/include/marl/event.h"
#include "marl/scheduler.h"
#include "marl/defer.h"

using namespace TD;

extern "C" {
void FillTOPPluginInfo(TOP_PluginInfo *info) {
    if (!info->setAPIVersion(TOPCPlusPlusAPIVersion)) {
        // TODO: handle unsupported version
    }

    // Change this to change the executeMode behavior of this plugin.
    info->executeMode = TOP_ExecuteMode::CPUMem;

    OP_CustomOPInfo &customInfo = info->customOPInfo;
    customInfo.opType->setString("Canvas");
    customInfo.opLabel->setString("Canvas");
    customInfo.authorName->setString("sigil");
    customInfo.authorName->setString("me@cybersigilim.space");

    // This TOP takes N inputs
    customInfo.minInputs = 1;
    customInfo.maxInputs = 1;
}

void DestroyTOPInstance(TOP_CPlusPlusBase *instance, TOP_Context *context) {
    delete (SigilTextPlugin *)instance;
}

TOP_CPlusPlusBase *CreateTOPInstance(const OP_NodeInfo *info,
                                     TOP_Context *context) {
    return new SigilTextPlugin(info, context);
}
}

SigilTextPlugin::SigilTextPlugin(const OP_NodeInfo *info, TOP_Context *context)
    : m_context{context}, m_info{info} {
    textSystem_ = std::make_unique<TextSystem>(scene_);
}

SigilTextPlugin::~SigilTextPlugin() {}

void SigilTextPlugin::getGeneralInfo(TOP_GeneralInfo *ginfo,
                                     const OP_Inputs *inputs, void *reserved1) {
    ginfo->cookEveryFrame = false;
    ginfo->inputSizeIndex = 0;
}

void SigilTextPlugin::execute(TOP_Output *output, const OP_Inputs *inputs,
                              void *reserved) {
    static marl::Scheduler scheduler{marl::Scheduler::Config::allCores()};
    // Prep required data
    OP_TextureDesc texture_desc;
    output->getSuggestedOutputDesc(&texture_desc, nullptr);

    // Prep renderer
    TDBuffer output_buffer{m_context};
    Renderer renderer{Renderer::RenderConfig{texture_desc.width, texture_desc.height, false, true}, output_buffer};

    // Update the scene
    auto &text = scene_.text;
    text.style.typeFamily = parameters_.evalTypefamily(inputs);
    text.style.fontSize = parameters_.evalFontsize(inputs);
    const TD::Color foreground_color = parameters_.evalForegroundcolor(inputs);
    text.style.foregroundColor = {foreground_color.r, foreground_color.g, foreground_color.b, foreground_color.a};
    text.content.content = parameters_.evalContent(inputs);

    std::thread thread([&] {
        scheduler.bind();
        defer(scheduler.unbind());

        textSystem_->update(renderer.width());

        // Render
        renderer.render(scene_);
    });

    thread.join();

    // Upload the texture
    TOP_UploadInfo uploadInfo = {};
    uploadInfo.textureDesc.width = renderer.width();
    uploadInfo.textureDesc.height = renderer.height();
    uploadInfo.textureDesc.pixelFormat = OP_PixelFormat::BGRA8Fixed;
    uploadInfo.textureDesc.texDim = OP_TexDim::e2D;
    uploadInfo.firstPixel = TOP_FirstPixel::TopLeft;
    uploadInfo.colorBufferIndex = 0;
    output->uploadBuffer(output_buffer.useTOPBuffer(), uploadInfo, nullptr);
}

void SigilTextPlugin::setupParameters(OP_ParameterManager *manager,
                                      void *reserved1) {
    parameters_.setup(manager);
}
