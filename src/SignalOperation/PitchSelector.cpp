#include "SignalOperation/PitchSelector.hpp"

#include "Audio/Signal.hpp"

#include "Audio/PcmData.hpp"

#include "App/App.hpp"

#include <array>

#include "imgui.h"

//--------------------------------------------------------------
PitchSelector::PitchSelector()
{
    // initialize({DataType_Float,DataType_Float},{DataType_Float});
    makeInput("octave", DataType_Float);
    makeInput("semitone", DataType_Float);
    makeOutput("freq", DataType_Float);
    makeProperty({"octave", DataType_Int, &octave});
    makeProperty({"semitone", DataType_Int, &semitone});
}
//--------------------------------------------------------------
void PitchSelector::validate()
{
}

//--------------------------------------------------------------
int PitchSelector::getMidiIndex() const
{
    constexpr int midi_first_octave = -1;
    constexpr int midi_semitones = 12;
    return (octave-midi_first_octave) * midi_semitones + semitone;
}

//--------------------------------------------------------------
float PitchSelector::getFreq() const
{
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    return pitchToFreq((float)getMidiIndex(), midi_la440, midi_semitones);
}

//--------------------------------------------------------------
OperationData PitchSelector::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    auto output1  = getOutput(0);
    OperationData data;
    data.type = output1->type;
    data.count = output1->count;

    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float oc = a.type == DataType_Float ? a.fvec[0] : octave;
    float se = b.type == DataType_Float ? b.fvec[0] : semitone;
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    constexpr int midi_first_octave = -1;
    float midiindex = (oc-midi_first_octave) * midi_semitones + se;
    data.fvec[0] = pitchToFreq(midiindex, midi_la440, midi_semitones);

    return data;
}


//--------------------------------------------------------------
void PitchSelector::uiProperties()
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


    if (ImGui::InputInt("octave", &octave)) preview.dirty();
    if (ImGui::InputInt("semitone", &semitone)) preview.dirty();
    if (ImGui::Button("Play") && App::s_instance)
    {
        auto& sound = *(App::s_instance->sound);
        if(sound.getState() != SoundNode::Playing)
        {
            qb::Pcm16 pcm = generator(getFreq());
            sound.queue(pcm);
            sound.play();
        }
    }

    
    constexpr std::array<const char*, 12> pitch_name = {"C", "C#/Db", "D", "Eb/D#", "E", "F", "F#/Gb", "G", "Ab/G#", "A", "Bb/A#", "B"};
    ImGui::Separator();
    ImGui::Text("info:");
    int midi = getMidiIndex();
    const char* noteName = pitch_name[midi % 12];
    float freq = getFreq();
    ImGui::Text(noteName);
    ImGui::InputInt("midi", &midi);
    ImGui::InputFloat("freq", &freq);
    ImGui::Separator();
    ImGui::Text("Preview");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}