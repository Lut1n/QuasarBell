#include "gui/nodal/FloatSignalNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, FloatSignalNode> float_node_factory("float");

//--------------------------------------------------------------
FloatSignalNode::FloatSignalNode()
    : SignalOperationNode("Float", qb::OperationType_Float)
{
    addPin(0, "value", true);
    setOperation(&floatInput);
}
//--------------------------------------------------------------
void FloatSignalNode::displayProperties()
{
    ImGui::InputFloat("value", &floatInput.value);
    displayPreview();
}