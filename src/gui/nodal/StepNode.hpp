#ifndef GUI_STEP_NODE_H
#define GUI_STEP_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/StepOperation.hpp"

struct StepNode : public SignalOperationNode
{
    StepNode();

    void displayProperties() override;
    
public:
    StepOperation stepOperation;
};

#endif // GUI_STEP_NODE_H
