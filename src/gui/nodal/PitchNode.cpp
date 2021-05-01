#include "gui/nodal/PitchNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "signal/Signal.hpp"

#include "imgui.h"

#include "app/App.hpp"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, PitchNode> pitch_node_factory(qb::OperationType_Pitch);

//--------------------------------------------------------------
PitchNode::PitchNode()
    : SignalOperationNode("Pitch", qb::OperationType_Pitch)
{
    addPin(0, "freq", true);
    addPin(0, "octave", false);
    addPin(1, "semitone", false);
    setOperation(&pitch);
}

//--------------------------------------------------------------
void PitchNode::displayProperties()
{
    auto generator = [](float f){
        float duration = 0.3f;
        auto& settings = AudioSettings::defaultSettings;
        qb::Pcm16 output;
        output.resize((size_t)(duration * settings.sampleRate));
        for(unsigned i=0;i<output.count();++i)
        {
            float t = (float)i / (float)settings.sampleRate;
            output.set(i, std::sin(f * t * 2.f*3.141592f));
        }
        return output;
    };


    if (ImGui::InputInt("octave", &pitch.octave)) dirtyPreview();
    if (ImGui::InputInt("semitone", &pitch.semitone)) dirtyPreview();
    if (ImGui::Button("Play") && App::s_instance)
    {
        auto& sound = *(App::s_instance->sound);
        if(sound.getState() != SoundNode::Playing)
        {
            qb::Pcm16 pcm = generator(pitch.getFreq());
            sound.queue(pcm);
            sound.play();
        }
    }

    
    constexpr std::array<const char*, 12> pitch_name = {"C", "C#/Db", "D", "Eb/D#", "E", "F", "F#/Gb", "G", "Ab/G#", "A", "Bb/A#", "B"};
    ImGui::Separator();
    ImGui::Text("info:");
    int midi = pitch.getMidiIndex();
    const char* noteName = pitch_name[midi % 12];
    float freq = pitch.getFreq();
    ImGui::Text(noteName);
    ImGui::InputInt("midi", &midi);
    ImGui::InputFloat("freq", &freq);
    displayPreview();
}