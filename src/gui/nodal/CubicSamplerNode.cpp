#include "gui/nodal/CubicSamplerNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, CubicSamplerNode> linear_node_factory("sampler");

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
    ImGui::InputFloat("value", &sampler.value);
    ImGui::InputFloat("speed", &sampler.speed);
    ImGui::InputFloat("acc", &sampler.acc);
    ImGui::InputFloat("jerk", &sampler.jerk);
    ImGui::InputFloat("reset", &sampler.reset);
    displayPreview();
}