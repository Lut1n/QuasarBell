#ifndef GUI_POLY_SAMPLER_NODE_H
#define GUI_POLY_SAMPLER_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"

struct PolynomialNode : public SignalOperationNode
{
    PolynomialNode();

    void displayProperties() override;
    
public:
    PolynomialSampler sampler;
};

#endif // GUI_POLY_SAMPLER_NODE_H
