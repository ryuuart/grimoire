#include "Gui.h"
#include "ToolCanvas.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include <cfloat>

Gui::Gui(ToolContext &context) : m_context(context) {}

void Gui::draw() {
    ImGui::Begin("Testerino Imgui window 1");
    ImGui::Text("Texterino pepperino");
    ImGui::Text("Texterino pepperino");
    ImGui::Text("Texterino pepperino");
    ImGui::Text("Texterino pepperino");
    ImGui::End();

    ImGui::Begin("Testerino Imgui window 2");

    ImGui::Text("clocck: %llu", m_context.totalTime);
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_WordWrap;
    ImGui::InputTextMultiline("Content", &m_context.content,
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                              flags);

    ImGui::End();
}
