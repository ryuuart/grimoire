#include "Plugin.h"
#include "CPlusPlus_Common.h"
#include "TOP_CPlusPlusBase.h"
#include "Typography.h"
#include "modules/skshaper/include/SkShaper.h"
#include "modules/skshaper/include/SkShaper_harfbuzz.h"
#include "modules/skshaper/include/SkShaper_skunicode.h"
#include "modules/skunicode/include/SkUnicode_icu.h"
#include <cstddef>

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
    ginfo->cookEveryFrame = false;
    ginfo->inputSizeIndex = 0;
}

void SigilTextPlugin::execute(TOP_Output *output, const OP_Inputs *inputs,
                              void *reserved) {
    // Prep required data
    OP_TextureDesc texture_desc;
    output->getSuggestedOutputDesc(&texture_desc, nullptr);

    // Update the scene
    auto &text = scene_.text;
    text.style->typeface =
        Typography::getInstance().getFontManager()->matchFamilyStyle(
            parameters_.evalTypefamily(inputs).c_str(), SkFontStyle{});
    text.style->font =
        SkFont{text.style->typeface,
               static_cast<float>(parameters_.evalFontsize(inputs))};
    TD::Color foreground_color = parameters_.evalForegroundcolor(inputs);
    text.style->foregroundColor =
        SkPaint{SkColor4f{foreground_color.r, foreground_color.g,
                          foreground_color.b, foreground_color.a}};
    std::string current_content = parameters_.evalContent(inputs);
    text.content->content = current_content;
    auto run_handler =
        SkTextBlobBuilderRunHandler(text.content->content.c_str(), {0, 0});

    text.shaper->unicode = SkUnicodes::ICU::Make();
    std::string &content = text.content->content;
    std::size_t size = content.size();
    auto font = SkShaper::MakeFontMgrRunIterator(
        content.c_str(), size, text.style->font,
        Typography::getInstance().getFontManager());
    auto bidi = SkShapers::unicode::BidiRunIterator(
        text.shaper->unicode, content.c_str(), size, SkBidiIterator::kLTR);
    auto language = SkShaper::MakeStdLanguageRunIterator(content.c_str(), size);
    auto script = SkShapers::HB::ScriptRunIterator(content.c_str(), size);
    text.shaper->shaper->shape(content.c_str(), size, *font, *bidi, *script,
                               *language, nullptr, 0, texture_desc.width,
                               &run_handler);
    text.shaper->textBlob = run_handler.makeBlob();
    renderer_.resize(texture_desc.width, texture_desc.height);
    renderer_.render(scene_);

    // Upload the texture
    OP_SmartRef<TOP_Buffer> buffer = m_context->createOutputBuffer(
        renderer_.byteSize(), TOP_BufferFlags::Readable, nullptr);
    renderer_.readPixel(buffer->data);
    TOP_UploadInfo uploadInfo = {};
    uploadInfo.textureDesc.width = renderer_.width();
    uploadInfo.textureDesc.height = renderer_.height();
    uploadInfo.textureDesc.pixelFormat = OP_PixelFormat::BGRA8Fixed;
    uploadInfo.textureDesc.texDim = OP_TexDim::e2D;
    uploadInfo.firstPixel = TOP_FirstPixel::TopLeft;
    uploadInfo.colorBufferIndex = 0;
    output->uploadBuffer(&buffer, uploadInfo, nullptr);
}

void SigilTextPlugin::setupParameters(OP_ParameterManager *manager,
                                      void *reserved1) {
    parameters_.setup(manager);
}
