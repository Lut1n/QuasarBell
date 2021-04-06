#include "gui/nodal/FloatSignalNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
FloatSignalNode::FloatSignalNode(const vec2& position)
    : SignalOperationNode("Float Input", position)
{
    addPin(0, "value", true);
    setOperation(&floatInput);
}
//--------------------------------------------------------------
void FloatSignalNode::displayProperties()
{
    ImGui::InputFloat("value", &floatInput.value);
}