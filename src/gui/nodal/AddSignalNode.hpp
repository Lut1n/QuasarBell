#ifndef GUI_ADD_SIGNAL_NODE_H
#define GUI_ADD_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "UI/UiNode.h"
#include "signal/Operations.hpp"

struct AddSignalNode : public UiNode
{
    AddSignalNode(const vec2& position);

    void displayProperties() override;
    
public:
    AddOperation operation;
};

#endif // GUI_ADD_SIGNAL_NODE_H
