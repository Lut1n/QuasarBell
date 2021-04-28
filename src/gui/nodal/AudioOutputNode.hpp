#ifndef GUI_AUDIO_OUT_NODE_H
#define GUI_AUDIO_OUT_NODE_H

#include <memory>
#include <unordered_map>

#include "gui/nodal/SignalOperationNode.hpp"
#include "signal/operations/Operations.hpp"
#include "audio/AudioRenderer.hpp"

struct AudioOutputNode : public SignalOperationNode
{
    AudioOutputNode();
    virtual ~AudioOutputNode();

    void displayProperties() override;

    void generate(PcmDataBase& pcm);
    
public:
    OutputOperation output;
    float value = 0.0f;
    int sampleRateIndex = -1;
    int sampleFormatIndex = -1;
    
    static AudioOutputNode* defaultOutput;
};

#endif // GUI_AUDIO_OUT_NODE_H
