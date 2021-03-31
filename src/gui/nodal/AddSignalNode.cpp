#include "gui/nodal/AddSignalNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
AddSignalNode::AddSignalNode(const vec2& position)
    : UiNode("Add", position, vec2(100, 100))
{
    addPin(0, "in 1", false);
    addPin(1, "in 2", false);
    addPin(0, "out", true);
}
//--------------------------------------------------------------
void AddSignalNode::displayProperties()
{
}