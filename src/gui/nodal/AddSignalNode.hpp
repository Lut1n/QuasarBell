#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct AddSignalNode : public SignalOperationNode
{
    AddSignalNode(const vec2& position);

    void displayProperties() override;
    
public:
    AddOperation add;
};

struct MultSignalNode : public SignalOperationNode
{
    MultSignalNode(const vec2& position);

    void displayProperties() override;
    
public:
    MultOperation mult;
};

#endif // GUI_ADD_SIGNAL_NODE_H
