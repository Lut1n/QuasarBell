#include "gui/nodal/FloatSignalNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, FloatSignalNode> float_node_factory(qb::OperationType_Float);
static TypedFactory<SignalOperationNode, NoiseSignalNode> noise_node_factory(qb::OperationType_Noise);

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
    if (ImGui::InputFloat("value", &floatInput.value)) dirtyPreview();
    displayPreview();
}

//--------------------------------------------------------------
NoiseSignalNode::NoiseSignalNode()
    : SignalOperationNode("Noise", qb::OperationType_Noise)
{
    addPin(0, "value", true);
    setOperation(&input);
}
//--------------------------------------------------------------
void NoiseSignalNode::displayProperties()
{
}