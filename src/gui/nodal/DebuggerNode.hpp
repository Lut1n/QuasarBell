#ifndef GUI_DEBUG_SIGNAL_NODE_H
#define GUI_DEBUG_SIGNAL_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"
#include "render/AudioRenderer.hpp"

struct DebuggerNode : public SignalOperationNode
{
    DebuggerNode();
    virtual ~DebuggerNode();

    void draw() override;
    void displayProperties() override;

    void generate(PcmDataBase& pcm);
    
public:
    OutputOperation debug;
    float value = 0.0f;
    int sampleRateIndex = -1;
    int sampleFormatIndex = -1;
    
    static DebuggerNode* defaultOutput;
};

#endif // GUI_DEBUG_SIGNAL_NODE_H
