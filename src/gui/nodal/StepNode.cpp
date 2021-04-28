#include "gui/nodal/StepNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, StepNode> step_node_factory(qb::OperationType_Step);

//--------------------------------------------------------------
StepNode::StepNode()
    : SignalOperationNode("Step", qb::OperationType_Step)
{
    addPin(0, "signal", true);
    addPin(0, "test", false);
    addPin(1, "edge", false);
    addPin(2, "then", false);
    addPin(3, "else", false);
    setOperation(&stepOperation);
}
//--------------------------------------------------------------
void StepNode::displayProperties()
{
    ImGui::InputFloat("edge", &stepOperation.edge);
    ImGui::InputFloat("then", &stepOperation.thenValue);
    ImGui::InputFloat("else", &stepOperation.elseValue);
    displayPreview();
}