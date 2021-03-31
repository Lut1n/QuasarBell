#ifndef GUI_FLOAT_SIGNAL_NODE_H
#define GUI_FLOAT_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "UI/UiNode.h"
#include "signal/Operations.hpp"

struct FloatSignalNode : public UiNode
{
    FloatSignalNode(const vec2& position);

    void displayProperties() override;
    
public:
    FloatInput operation;
};

#endif // GUI_FLOAT_SIGNAL_NODE_H
