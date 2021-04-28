#include "gui/nodal/CubicSamplerNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, CubicSamplerNode> linear_node_factory(qb::OperationType_CubicSampler);

//--------------------------------------------------------------
CubicSamplerNode::CubicSamplerNode()
    : SignalOperationNode("Cubic", qb::OperationType_CubicSampler)
{
    addPin(0, "value", true);
    addPin(0, "value", false);
    addPin(1, "speed", false);
    addPin(2, "acc", false);
    addPin(3, "jerk", false);
    addPin(4, "reset", false);
    setOperation(&sampler);
}
//--------------------------------------------------------------
void CubicSamplerNode::displayProperties()
{
    if (ImGui::InputFloat("value", &sampler.value)) dirtyPreview();
    if (ImGui::InputFloat("speed", &sampler.speed)) dirtyPreview();
    if (ImGui::InputFloat("acc", &sampler.acc)) dirtyPreview();
    if (ImGui::InputFloat("jerk", &sampler.jerk)) dirtyPreview();
    if (ImGui::InputFloat("reset", &sampler.reset)) dirtyPreview();
    displayPreview();
}