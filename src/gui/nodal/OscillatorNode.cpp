#include "gui/nodal/OscillatorNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, OscillatorNode> oscillator_node_factory(qb::OperationType_Oscillator);

//--------------------------------------------------------------
OscillatorNode::OscillatorNode()
    : SignalOperationNode("Oscillator", qb::OperationType_Oscillator)
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
    if (ImGui::InputFloat("freq", &oscillator.freq)) dirtyPreview();
    if (ImGui::InputFloat("ampl", &oscillator.ampl)) dirtyPreview();
    displayPreview();
}