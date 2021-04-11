#ifndef GUI_MIX_NODE_H
#define GUI_MIX_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/MixOperation.hpp"

struct MixNode : public SignalOperationNode
{
    MixNode(const vec2& position);

    void displayProperties() override;
    
public:
    MixOperation mixer;
};

#endif // GUI_MIX_NODE_H
