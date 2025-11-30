#include "CanvasTOP.h"
#include "Canvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkSurface.h"
#include <CPlusPlus_Common.h>
#include <TOP_CPlusPlusBase.h>
#include <include/core/SkMatrix.h>
#include <optional>

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
    : m_canvas{128, 128}, m_context{context}, m_info{info} {}

CanvasTOP::~CanvasTOP() {}

void CanvasTOP::getGeneralInfo(TOP_GeneralInfo *ginfo, const OP_Inputs *inputs,
                               void *reserved1) {
    ginfo->cookEveryFrame = true;
    ginfo->inputSizeIndex = 0;
}

void CanvasTOP::execute(TOP_Output *output, const OP_Inputs *inputs,
                        void *reserved) {
    std::optional<float> scaleValue = std::nullopt;
    const OP_CHOPInput *scaleInput = m_parameters.evalScale(inputs);
    if (scaleInput) {
        scaleValue =
            scaleInput->getChannelData(0)[0]
                ? std::optional<float>(scaleInput->getChannelData(0)[0])
                : std::nullopt;
    }
    int count = m_parameters.evalCount(inputs);
    double rotation = m_parameters.evalRotation(inputs);
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
        for (int i = 0; i < count; i++) {
            float xrand = rand() % 10;
            float yrand = rand() % 10;
            // path.transform(SkMatrix::Translate(
            //     SkVector{0.5f * scale * xrand, 0.5f * scale * yrand}));
            path.transform(
                SkMatrix::Translate(SkVector{0.5f * scale, 0.5f * scale}));
            canvas->drawPath(path.detach(), p);
        }
        auto tmpScale = scaleValue.value_or(1.0f);
        canvas->scale(tmpScale, tmpScale);
        canvas->rotate(rotation);
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
    uploadInfo.textureDesc.pixelFormat = OP_PixelFormat::BGRA8Fixed;
    uploadInfo.textureDesc.texDim = OP_TexDim::e2D;
    uploadInfo.firstPixel = TOP_FirstPixel::TopLeft;
    uploadInfo.colorBufferIndex = 0;

    output->uploadBuffer(&buffer, uploadInfo, nullptr);
}

void CanvasTOP::setupParameters(OP_ParameterManager *manager, void *reserved1) {
    m_parameters.setup(manager);
}
