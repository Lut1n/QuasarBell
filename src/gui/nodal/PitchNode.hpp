#ifndef GUI_PITCH_NODE_H
#define GUI_PITCH_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/PitchSelector.hpp"

struct PitchNode : public SignalOperationNode
{
    PitchNode();

    void displayProperties() override;
    
public:
    PitchSelector pitch;
};

#endif // GUI_PITCH_NODE_H
