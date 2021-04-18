#ifndef GUI_FLOAT_CUBIC_NODE_H
#define GUI_FLOAT_CUBIC_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct CubicSamplerNode : public SignalOperationNode
{
    CubicSamplerNode();

    void displayProperties() override;
    
public:
    CubicSampler sampler;
};

#endif // GUI_FLOAT_CUBIC_NODE_H
