#include "gui/nodal/HarmonicsNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, HarmonicsNode> harmonics_node_factory(qb::OperationType_Harmonics);

//--------------------------------------------------------------
HarmonicsNode::HarmonicsNode()
    : SignalOperationNode("Harmonics", qb::OperationType_Harmonics)
{
    addPin(0, "value", true);
    setOperation(&sampler);
}
//--------------------------------------------------------------
void HarmonicsNode::displayProperties()
{
    if (ImGui::InputInt("count", &sampler.count))
    {
        if (sampler.count < 1) sampler.count = 1;
        if (sampler.count > 10) sampler.count = 10;
        sampler.freqs.resize(sampler.count);
        dirtyPreview();
    }
    
    ImGui::Columns(2);
    ImGui::Text("Freq");
    ImGui::NextColumn();
    ImGui::Text("Ampl");
    ImGui::NextColumn();
    ImGui::Separator();
    int index = 0;
    for(auto& kv : sampler.freqs)
    {
        std::string keytext = std::string("##key") + std::to_string(index);
        std::string valtext = std::string("##val") + std::to_string(index);
        if (ImGui::InputFloat(keytext.c_str(), &kv.first)) dirtyPreview();
        ImGui::NextColumn();
        if (ImGui::InputFloat(valtext.c_str(), &kv.second)) dirtyPreview();
        ImGui::NextColumn();
        index++;
    }
    ImGui::Columns(1);
    displayPreview();
}