#ifndef GUI_ENVELOP_NODE_H
#define GUI_ENVELOP_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/EnvelopOperation.hpp"

struct EnvelopNode : public SignalOperationNode
{
    EnvelopNode(const vec2& position);

    void displayProperties() override;
    
public:
    EnvelopOperation envelop;
};

#endif // GUI_ENVELOP_NODE_H
