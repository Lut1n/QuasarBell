#include "gui/nodal/EnvelopNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

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
    if (ImGui::InputFloat("attack", &envelop.attack)) dirtyPreview();
    if (ImGui::InputFloat("decay", &envelop.decay)) dirtyPreview();
    if (ImGui::InputFloat("sustain", &envelop.sustain)) dirtyPreview();
    if (ImGui::InputFloat("release", &envelop.release)) dirtyPreview();
    displayPreview();
}