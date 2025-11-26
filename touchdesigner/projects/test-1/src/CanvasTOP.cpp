#include "CanvasTOP.h"
#include "Canvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkSurface.h"
#include <CPlusPlus_Common.h>
#include <TOP_CPlusPlusBase.h>

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
    delete (CanvasTOP *)instance;
}

TOP_CPlusPlusBase *CreateTOPInstance(const OP_NodeInfo *info,
                                     TOP_Context *context) {
    return new CanvasTOP(info, context);
}
}

CanvasTOP::CanvasTOP(const OP_NodeInfo *info, TOP_Context *context)
    : m_canvas{640, 480}, m_context{context}, m_info{info} {}

CanvasTOP::~CanvasTOP() {}

void CanvasTOP::getGeneralInfo(TOP_GeneralInfo *ginfo, const OP_Inputs *inputs,
                               void *reserved1) {
    ginfo->cookEveryFrame = false;
    ginfo->inputSizeIndex = 0;
}

void CanvasTOP::execute(TOP_Output *output, const OP_Inputs *inputs,
                        void *reserved) {
    // if (inputs->getNumInputs() < 1) {
    //     spdlog::warn("CanvasTOP: No input connected.");
    //     return;
    // }

    // const OP_TOPInput *input = inputs->getInputTOP(0);
    // if (input == nullptr || input->width == 0 || input->height == 0) {
    //     spdlog::warn("CanvasTOP: Invalid input TOP.");
    //     return;
    // }

    m_canvas.draw([&](SkCanvas *canvas) {
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
        path.transform(
            SkMatrix::Translate(SkVector{0.5f * scale, 0.5f * scale}));
        canvas->drawPath(path.detach(), p);
    });

    const SkPixmap &pixmap = m_canvas.getPixmap();

    OP_SmartRef<TOP_Buffer> buffer = m_context->createOutputBuffer(
        pixmap.computeByteSize(), TOP_BufferFlags::Readable, nullptr);
    buffer->size = pixmap.computeByteSize();
    buffer->flags = TOP_BufferFlags::Readable;
    std::memcpy(buffer->data, pixmap.addr(), buffer->size);

    TOP_UploadInfo uploadInfo = {};
    uploadInfo.textureDesc.width = pixmap.width();
    uploadInfo.textureDesc.height = pixmap.height();
    uploadInfo.textureDesc.pixelFormat = OP_PixelFormat::RGBA8Fixed;
    uploadInfo.textureDesc.texDim = OP_TexDim::e2D;
    uploadInfo.firstPixel = TOP_FirstPixel::TopLeft;
    uploadInfo.colorBufferIndex = 0;

    output->uploadBuffer(&buffer, uploadInfo, nullptr);
}

void CanvasTOP::setupParameters(OP_ParameterManager *manager, void *reserved1) {
    // No custom parameters for now
}
