#ifndef GUI_DEBUG_SIGNAL_NODE_H
#define GUI_DEBUG_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"
#include "render/AudioRenderer.hpp"

struct DebuggerNode : public SignalOperationNode
{
    DebuggerNode(const vec2& position);

    void draw() override;
    void displayProperties() override;

    PcmData generate();
    
public:
    OutputOperation debug;
    float value = 0.0f;
    float range = 1.0f;
    float duration = 2.0;
};

#endif // GUI_DEBUG_SIGNAL_NODE_H
