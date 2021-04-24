#include "gui/nodal/KeySamplerNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, KeySamplerNode> keysampler_node_factory(qb::OperationType_KeySampler);

//--------------------------------------------------------------
KeySamplerNode::KeySamplerNode()
    : SignalOperationNode("Key-Sampler", qb::OperationType_KeySampler)
{
    addPin(0, "value", true);
    setOperation(&sampler);
}
//--------------------------------------------------------------
void KeySamplerNode::displayProperties()
{
    constexpr size_t count = 3;
    constexpr std::array<const char*, count> typeNames = {"flat", "linear", "cubic"};
    if (ImGui::Button(typeNames[sampler.interpo]))
    {
        sampler.interpo = (sampler.interpo+1) % count;
    }

    if (ImGui::InputInt("count", &sampler.count))
    {
        if (sampler.count < 1) sampler.count = 1;
        if (sampler.count > 10) sampler.count = 10;
        sampler.keys.resize(sampler.count);
    }
    
    ImGui::Columns(2);
    ImGui::Text("Time");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Separator();
    int index = 0;
    for(auto& kv : sampler.keys)
    {
        std::string keytext = std::string("##key") + std::to_string(index);
        std::string valtext = std::string("##val") + std::to_string(index);
        ImGui::InputFloat(keytext.c_str(), &kv.first);
        ImGui::NextColumn();
        ImGui::InputFloat(valtext.c_str(), &kv.second);
        ImGui::NextColumn();
        index++;
    }
    ImGui::Columns(1);
    displayPreview();
}