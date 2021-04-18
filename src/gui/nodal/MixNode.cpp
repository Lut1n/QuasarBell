#include "gui/nodal/MixNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, MixNode> mix_node_factory("mix");

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
    ImGui::InputFloat("input1", &mixer.input1);
    ImGui::InputFloat("input2", &mixer.input2);
    ImGui::InputFloat("delta", &mixer.delta);
    displayPreview();
}