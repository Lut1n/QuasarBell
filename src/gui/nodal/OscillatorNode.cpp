#include "gui/nodal/OscillatorNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
OscillatorNode::OscillatorNode(const vec2& position)
    : SignalOperationNode("Oscillator", position)
{
    addPin(0, "signal", true);
    addPin(0, "freq", false);
    addPin(1, "ampl", false);
    addPin(2, "waveform", false);
    setOperation(&oscillator);
}
//--------------------------------------------------------------
void OscillatorNode::displayProperties()
{
    ImGui::InputFloat("freq", &oscillator.freq);
    ImGui::InputFloat("ampl", &oscillator.ampl);

    
    // ImGui::Text();
}