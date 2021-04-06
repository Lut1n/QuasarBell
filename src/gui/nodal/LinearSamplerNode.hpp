#ifndef GUI_FLOAT_LINEAR_NODE_H
#define GUI_FLOAT_LINEAR_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct LinearSamplerNode : public SignalOperationNode
{
    LinearSamplerNode(const vec2& position);

    void displayProperties() override;
    
public:
    LinearInput linInput;
};

#endif // GUI_FLOAT_LINEAR_NODE_H
