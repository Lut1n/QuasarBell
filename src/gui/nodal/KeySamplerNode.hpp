#ifndef GUI_KEY_SAMPLER_NODE_H
#define GUI_KEY_SAMPLER_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/KeySampler.hpp"

struct KeySamplerNode : public SignalOperationNode
{
    KeySamplerNode();

    void displayProperties() override;
    
public:
    KeySampler sampler;
};

#endif // GUI_KEY_SAMPLER_NODE_H
