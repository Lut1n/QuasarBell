#include "gui/nodal/EnvelopNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, EnvelopNode> envelop_node_factory(qb::OperationType_Envelop);

//--------------------------------------------------------------
EnvelopNode::EnvelopNode()
    : SignalOperationNode("ADSR", qb::OperationType_Envelop)
{
    addPin(0, "value", true);
    setOperation(&envelop);
}
//--------------------------------------------------------------
void EnvelopNode::displayProperties()
{
    ImGui::InputFloat("attack", &envelop.attack);
    ImGui::InputFloat("decay", &envelop.decay);
    ImGui::InputFloat("sustain", &envelop.sustain);
    ImGui::InputFloat("release", &envelop.release);
    displayPreview();
}