#include "gui/nodal/LinearSamplerNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
LinearSamplerNode::LinearSamplerNode(const vec2& position)
    : SignalOperationNode("Linear Input", position)
{
    addPin(0, "value", true);
    addPin(0, "value", false);
    addPin(1, "speed", false);
    addPin(2, "acc", false);
    setOperation(&linInput);
}
//--------------------------------------------------------------
void LinearSamplerNode::displayProperties()
{
    ImGui::InputFloat("value", &linInput.value);
    ImGui::InputFloat("speed", &linInput.speed);
    ImGui::InputFloat("acc", &linInput.acc);
    displayPreview();
}