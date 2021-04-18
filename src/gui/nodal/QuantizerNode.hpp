#ifndef GUI_QUANTIZER_NODE_H
#define GUI_QUANTIZER_NODE_H

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Quantizer.hpp"

struct QuantizerNode : public SignalOperationNode
{
    QuantizerNode();

    void displayProperties() override;
    
public:
    Quantizer quantizer;
};

#endif // GUI_QUANTIZER_NODE_H
