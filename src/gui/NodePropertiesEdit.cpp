#include "gui/NodePropertiesEdit.hpp"

#include "imgui.h"

#include "ui/UiNode.h"

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
    if(!UiNode::selected.empty())
        UiNode::selected.front()->displayProperties();
    ImGui::End();
}
