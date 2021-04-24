#include "gui/nodal/QuantizerNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, QuantizerNode> quantizer_node_factory(qb::OperationType_Quantizer);

//--------------------------------------------------------------
QuantizerNode::QuantizerNode()
    : SignalOperationNode("Quantizer", qb::OperationType_Quantizer)
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