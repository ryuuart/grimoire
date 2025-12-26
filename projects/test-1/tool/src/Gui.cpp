#include "Gui.h"
#include "ToolCanvas.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <cfloat>
#include <cstdint>

Gui::Gui(ToolContext &context) : m_context(context) {}

void Gui::draw() {
    ImGui::Begin("Controls");
    static uint8_t step_size = 1;
    static double step_float_size = 1.0f;
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_WordWrap;
    ImGui::InputScalarN("Clones", ImGuiDataType_U16, &m_context.clones_xy, 2,
                        &step_size, NULL, "%u", ImGuiInputTextFlags_None);
    ImGui::InputScalarN("Offset", ImGuiDataType_S64, &m_context.offset_xy, 2,
                        &step_size, NULL, "%ld", ImGuiInputTextFlags_None);
    ImGui::DragScalar("Translation", ImGuiDataType_U32, &m_context.translation);
    ImGui::InputScalar("Font Size", ImGuiDataType_U16, &m_context.fontSize,
                       &step_size, NULL, "%u", ImGuiInputTextFlags_None);
    ImGui::InputScalar("Content Width", ImGuiDataType_U32,
                       &m_context.contentWidth, &step_size, NULL, "%u",
                       ImGuiInputTextFlags_None);
    ImGui::InputScalar("Duration", ImGuiDataType_Double, &m_context.duration,
                       &step_float_size, NULL, NULL, ImGuiInputTextFlags_None);
    ImGui::Text("Content");
    ImGui::InputTextMultiline("Content", &m_context.content,
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                              flags);

    ImGui::End();
}
