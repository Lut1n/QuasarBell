#include "gui/nodal/DebuggerNode.hpp"

#include "signal/Signal.hpp"

#include "imgui.h"

#include "App.hpp"

#include <string>

//--------------------------------------------------------------
DebuggerNode::DebuggerNode(const vec2& position)
    : SignalOperationNode("Debugger", position)
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
static float s_imgui_sampler(void* data, int idx)
{
    SignalOperation& op = *(SignalOperation*)data;
    SignalOperation::Time time;
    time.duration = 1.0;
    time.t = (float)idx/100.0f;
    time.sec = (float)idx/100.0f;
    time.elapsed = 0.01;
    return op.sample(0, time).fvec[0];
}

//--------------------------------------------------------------
void DebuggerNode::displayProperties()
{
    getOperation()->validateGraph();
    
    ImGui::PlotLines("##preview", s_imgui_sampler, getOperation(), 100, 0, NULL, -range, range, ImVec2(0, 60.0f));
    std::string s = std::to_string(value);
    ImGui::Text(s.c_str());

    ImGui::InputFloat("range", &range);
    ImGui::InputFloat("duration scale", &duration);
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