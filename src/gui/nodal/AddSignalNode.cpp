#include "gui/nodal/AddSignalNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"
#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, AddSignalNode> add_node_factory("add");
static TypedFactory<SignalOperationNode, MultSignalNode> mult_node_factory("mult");

//--------------------------------------------------------------
AddSignalNode::AddSignalNode()
    : SignalOperationNode("Add", qb::OperationType_Add)
{
    addPin(0, "in 1", false);
    addPin(1, "in 2", false);
    addPin(0, "out", true);

    setOperation(&add);
}
//--------------------------------------------------------------
void AddSignalNode::displayProperties()
{
    ImGui::InputFloat("input1", &add.input1);
    ImGui::InputFloat("input2", &add.input2);
    displayPreview();
}


//--------------------------------------------------------------
MultSignalNode::MultSignalNode()
    : SignalOperationNode("Mult", qb::OperationType_Mult)
{
    addPin(0, "in 1", false);
    addPin(1, "in 2", false);
    addPin(0, "out", true);

    setOperation(&mult);
}
//--------------------------------------------------------------
void MultSignalNode::displayProperties()
{
    ImGui::InputFloat("input1", &mult.input1);
    ImGui::InputFloat("input2", &mult.input2);
    displayPreview();
}