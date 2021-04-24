#ifndef GUI_HARMONICS_NODE_H
#define GUI_HARMONICS_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Harmonics.hpp"

struct HarmonicsNode : public SignalOperationNode
{
    HarmonicsNode();

    void displayProperties() override;
    
public:
    Harmonics sampler;
};

#endif // GUI_HARMONICS_NODE_H
