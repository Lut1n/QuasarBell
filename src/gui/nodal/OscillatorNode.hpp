#ifndef GUI_OSCILLATOR_NODE_H
#define GUI_OSCILLATOR_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Oscillator.hpp"

struct OscillatorNode : public SignalOperationNode
{
    OscillatorNode();

    void displayProperties() override;
    
public:
    Oscillator oscillator;
};

#endif // GUI_OSCILLATOR_NODE_H
