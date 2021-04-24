#ifndef GUI_WAVEFORM_NODE_H
#define GUI_WAVEFORM_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Waveform.hpp"

struct WaveformNode : public SignalOperationNode
{
    WaveformNode();

    void displayProperties() override;
    
public:
    Waveform waveform;
};

#endif // GUI_WAVEFORM_NODE_H
