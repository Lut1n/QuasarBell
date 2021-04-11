#include "gui/nodal/QuantizerNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
QuantizerNode::QuantizerNode(const vec2& position)
    : SignalOperationNode("Quantizer", position)
{
    addPin(0, "signal", true);
    addPin(0, "signal", false);
    addPin(1, "quantity", false);
    setOperation(&quantizer);
}
//--------------------------------------------------------------
void QuantizerNode::displayProperties()
{
    ImGui::InputInt("quantity", &quantizer.quantity);
    displayPreview();
}