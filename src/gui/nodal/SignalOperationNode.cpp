#include "gui/nodal/SignalOperationNode.hpp"

//--------------------------------------------------------------
SignalOperationNode::SignalOperationNode(const std::string& title, const vec2& position)
    : UiNode(title, position, vec2(60,60))
{
}

//--------------------------------------------------------------
void SignalOperationNode::setOperation(SignalOperation* op)
{
    operation = op;
}

//--------------------------------------------------------------
SignalOperation* SignalOperationNode::getOperation()
{
    return operation;
}