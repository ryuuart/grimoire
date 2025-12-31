#include "Plugin.h"
#include "CPlusPlus_Common.h"
#include "TOP_CPlusPlusBase.h"

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
    : m_context{context}, m_info{info} {}

SigilTextPlugin::~SigilTextPlugin() {}

void SigilTextPlugin::getGeneralInfo(TOP_GeneralInfo *ginfo,
                                     const OP_Inputs *inputs, void *reserved1) {
    ginfo->cookEveryFrame = true;
    ginfo->inputSizeIndex = 0;
}

void SigilTextPlugin::execute(TOP_Output *output, const OP_Inputs *inputs,
                              void *reserved) {
    auto image_info = scene.getImageInfo();
    OP_SmartRef<TOP_Buffer> buffer = m_context->createOutputBuffer(
        image_info.computeByteSize(image_info.minRowBytes()),
        TOP_BufferFlags::Readable, nullptr);
    scene.render(buffer->data);
    // std::memcpy(buffer->data, pixmap.addr(), buffer->size);

    TOP_UploadInfo uploadInfo = {};
    uploadInfo.textureDesc.width = image_info.width();
    uploadInfo.textureDesc.height = image_info.height();
    uploadInfo.textureDesc.pixelFormat = OP_PixelFormat::BGRA8Fixed;
    uploadInfo.textureDesc.texDim = OP_TexDim::e2D;
    uploadInfo.firstPixel = TOP_FirstPixel::TopLeft;
    uploadInfo.colorBufferIndex = 0;

    output->uploadBuffer(&buffer, uploadInfo, nullptr);
}

void SigilTextPlugin::setupParameters(OP_ParameterManager *manager,
                                      void *reserved1) {
    m_parameters.setup(manager);
}
