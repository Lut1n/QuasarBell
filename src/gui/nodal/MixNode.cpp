#include "gui/nodal/MixNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, MixNode> mix_node_factory(qb::OperationType_Mix);

//--------------------------------------------------------------
MixNode::MixNode()
    : SignalOperationNode("Mix", qb::OperationType_Mix)
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
    if (ImGui::InputFloat("input1", &mixer.input1)) dirtyPreview();
    if (ImGui::InputFloat("input2", &mixer.input2)) dirtyPreview();
    if (ImGui::InputFloat("delta", &mixer.delta)) dirtyPreview();
    displayPreview();
}