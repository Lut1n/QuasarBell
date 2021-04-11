#include "gui/nodal/MixNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
MixNode::MixNode(const vec2& position)
    : SignalOperationNode("Mix", position)
{
    addPin(0, "value", true);
    addPin(0, "input1", false);
    addPin(1, "input2", false);
    addPin(2, "delta", false);
    setOperation(&mixer);
}
//--------------------------------------------------------------
void MixNode::displayProperties()
{
    ImGui::InputFloat("input1", &mixer.input1);
    ImGui::InputFloat("input2", &mixer.input2);
    ImGui::InputFloat("delta", &mixer.delta);
    displayPreview();
}