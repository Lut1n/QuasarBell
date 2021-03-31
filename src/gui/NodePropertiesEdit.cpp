#include "gui/NodePropertiesEdit.hpp"

#include "imgui.h"

#include "UI/UiNode.h"

//--------------------------------------------------------------
NodePropertiesEdit::NodePropertiesEdit()
{
}
//--------------------------------------------------------------
void NodePropertiesEdit::render()
{
    ImGui::SetNextWindowPos(ImVec2(6, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 530), ImGuiCond_FirstUseEver);
    ImGui::Begin("Node properties");
    if(UiNode::focused != nullptr)
        UiNode::focused->displayProperties();
    ImGui::End();
}
