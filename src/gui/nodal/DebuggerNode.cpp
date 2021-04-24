#include "gui/nodal/DebuggerNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "signal/Signal.hpp"

#include "imgui.h"

#include "App.hpp"

#include <string>

#include "Core/Factory.h"

static TypedFactory<SignalOperationNode, DebuggerNode> debugger_node_factory(qb::OperationType_Debug);

//--------------------------------------------------------------
DebuggerNode::DebuggerNode()
    : SignalOperationNode("Debugger", qb::OperationType_Debug)
{
    addPin(0, "in 1", false);
    setOperation(&debug);
}

void DebuggerNode::draw()
{
    getOperation()->validateGraph();
    SignalOperation::Time time;
    value = getOperation()->sample(0, time).fvec[0];
    title->text = std::to_string(value);
    UiNode::draw();
}

//--------------------------------------------------------------
void DebuggerNode::displayProperties()
{
    getOperation()->validateGraph();
    ImGui::PlotLines("##preview", s_imgui_sampler, getOperation(), 100, 0, NULL, -debug.range, debug.range, ImVec2(0, 60.0f));
    std::string s = std::to_string(value);
    ImGui::Text(s.c_str());

    ImGui::InputFloat("range", &debug.range);
    ImGui::InputFloat("duration scale", &debug.duration);
    if (ImGui::Button("Play") && App::s_instance)
    {
        auto& sound = *(App::s_instance->sound);
        if(sound.getState() != SoundNode::Playing)
        {
            PcmData pcm = generate();
            sound.queue(pcm);
            sound.play();
        }
    }
}

//--------------------------------------------------------------
PcmData DebuggerNode::generate()
{
    float duration = debug.duration;
    if (duration == 0.0f)
    {
        std::cout << "error: duration = 0; 1s is used" << std::endl;
        duration = 1.0;
    }

    auto& settings = AudioSettings::defaultSettings;
    auto* operations = getOperation();

    operations->validateGraph();

    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    short quantizer = std::numeric_limits<short>::max();
    
    float sample_t = 1.0 / settings.sampleRate;
    
    SignalOperation::Time time;
    time.duration = duration;
    time.elapsed = sample_t;
    for(unsigned i=0;i<output.samples.size();++i)
    {
        time.sec = (float)i / (float)settings.sampleRate;
        time.t = time.sec / time.duration;
        output.samples[i] = operations->sample(0, time).fvec[0] * quantizer;
    }
    return output;
}