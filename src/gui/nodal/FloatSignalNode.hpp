#ifndef GUI_FLOAT_SIGNAL_NODE_H
#define GUI_FLOAT_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct FloatSignalNode : public SignalOperationNode
{
    FloatSignalNode();

    void displayProperties() override;
    
public:
    FloatInput floatInput;
};

struct NoiseSignalNode : public SignalOperationNode
{
    NoiseSignalNode();

    void displayProperties() override;
    
public:
    NoiseInput input;
};

#endif // GUI_FLOAT_SIGNAL_NODE_H
