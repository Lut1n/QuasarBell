#include "gui/nodal/EnvelopNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
EnvelopNode::EnvelopNode(const vec2& position)
    : SignalOperationNode("ADSR", position)
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