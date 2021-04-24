#ifndef GUI_FILTER_NODE_H
#define GUI_FILTER_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/FilterOperation.hpp"

struct FilterNode : public SignalOperationNode
{
    FilterNode();

    void displayProperties() override;
    
public:
    FreqFilter filter;
};

#endif // GUI_FILTER_NODE_H
