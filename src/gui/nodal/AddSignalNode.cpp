#include "gui/nodal/AddSignalNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"
#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, AddSignalNode> add_node_factory(qb::OperationType_Add);
static TypedFactory<SignalOperationNode, SubSignalNode> sub_node_factory(qb::OperationType_Sub);
static TypedFactory<SignalOperationNode, MultSignalNode> mult_node_factory(qb::OperationType_Mult);
static TypedFactory<SignalOperationNode, DivSignalNode> div_node_factory(qb::OperationType_Div);
static TypedFactory<SignalOperationNode, ClampSignalNode> clamp_node_factory(qb::OperationType_Clamp);
static TypedFactory<SignalOperationNode, AbsSignalNode> abs_node_factory(qb::OperationType_Abs);
static TypedFactory<SignalOperationNode, TimeScaleNode> timescale_node_factory(qb::OperationType_TimeScale);

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
    if (ImGui::InputFloat("input1", &add.input1)) dirtyPreview();
    if (ImGui::InputFloat("input2", &add.input2)) dirtyPreview();
    displayPreview();
}

//--------------------------------------------------------------
SubSignalNode::SubSignalNode()
    : SignalOperationNode("Sub", qb::OperationType_Sub)
{
    addPin(0, "in 1", false);
    addPin(1, "in 2", false);
    addPin(0, "out", true);

    setOperation(&sub);
}
//--------------------------------------------------------------
void SubSignalNode::displayProperties()
{
    if (ImGui::InputFloat("input1", &sub.input1)) dirtyPreview();
    if (ImGui::InputFloat("input2", &sub.input2)) dirtyPreview();
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
    if (ImGui::InputFloat("input1", &mult.input1)) dirtyPreview();
    if (ImGui::InputFloat("input2", &mult.input2)) dirtyPreview();
    displayPreview();
}

//--------------------------------------------------------------
DivSignalNode::DivSignalNode()
    : SignalOperationNode("Div", qb::OperationType_Div)
{
    addPin(0, "in 1", false);
    addPin(1, "in 2", false);
    addPin(0, "out", true);

    setOperation(&div);
}
//--------------------------------------------------------------
void DivSignalNode::displayProperties()
{
    if (ImGui::InputFloat("input1", &div.input1)) dirtyPreview();
    if (ImGui::InputFloat("input2", &div.input2)) dirtyPreview();
    displayPreview();
}

//--------------------------------------------------------------
ClampSignalNode::ClampSignalNode()
    : SignalOperationNode("Clamp", qb::OperationType_Clamp)
{
    addPin(0, "in 1", false);
    addPin(1, "min", false);
    addPin(2, "max", false);
    addPin(0, "out", true);

    setOperation(&clamp);
}
//--------------------------------------------------------------
void ClampSignalNode::displayProperties()
{
    if (ImGui::InputFloat("value", &clamp.input1)) dirtyPreview();
    if (ImGui::InputFloat("min", &clamp.minVal)) dirtyPreview();
    if (ImGui::InputFloat("max", &clamp.maxVal)) dirtyPreview();
    displayPreview();
}

//--------------------------------------------------------------
AbsSignalNode::AbsSignalNode()
    : SignalOperationNode("Abs", qb::OperationType_Abs)
{
    addPin(0, "value", false);
    addPin(0, "value", true);

    setOperation(&abs);
}
//--------------------------------------------------------------
void AbsSignalNode::displayProperties()
{
    displayPreview();
}

//--------------------------------------------------------------
TimeScaleNode::TimeScaleNode()
    : SignalOperationNode("TimeScale", qb::OperationType_TimeScale)
{
    addPin(0, "value", false);
    addPin(0, "value", true);

    setOperation(&timeScale);
}
//--------------------------------------------------------------
void TimeScaleNode::displayProperties()
{
    displayPreview();
    if (ImGui::InputFloat("delay", &timeScale.delay)) dirtyPreview();
    if (ImGui::InputFloat("scale", &timeScale.scale)) dirtyPreview();
}