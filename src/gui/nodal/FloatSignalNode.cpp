#include "gui/nodal/FloatSignalNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
FloatSignalNode::FloatSignalNode(const vec2& position)
    : UiNode("Float Input", position, vec2(100, 100))
{
    addPin(0, "value", true);
}
//--------------------------------------------------------------
void FloatSignalNode::displayProperties()
{
    ImGui::InputFloat("value", &operation.value);
}