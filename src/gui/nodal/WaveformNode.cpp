#include "gui/nodal/WaveformNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, WaveformNode> waveform_node_factory(qb::OperationType_Waveform);

//--------------------------------------------------------------
WaveformNode::WaveformNode()
    : SignalOperationNode("Waveform", qb::OperationType_Waveform)
{
    addPin(0, "signal", true);
    setOperation(&waveform);
}
//--------------------------------------------------------------
void WaveformNode::displayProperties()
{
    ImGui::InputFloat("period", &waveform.period);
    ImGui::InputFloat("min", &waveform.minVal);
    ImGui::InputFloat("max", &waveform.maxVal);

    constexpr size_t count = 4;
    constexpr std::array<const char*, count> typeNames = {"sin", "saw", "square", "noise"};
    if (ImGui::Button(typeNames[waveform.type]))
    {
        waveform.type = (waveform.type+1) % count;
    }

    if (waveform.type == 3)
    {
        ImGui::InputInt("noise seed", &waveform.noiseSeed);
        ImGui::InputInt("noise size", &waveform.noiseSamples);
    }
    
    displayPreview();
}