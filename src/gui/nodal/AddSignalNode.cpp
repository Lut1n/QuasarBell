#include "gui/nodal/AddSignalNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
AddSignalNode::AddSignalNode(const vec2& position)
    : SignalOperationNode("Add", position)
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
MultSignalNode::MultSignalNode(const vec2& position)
    : SignalOperationNode("Mult", position)
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