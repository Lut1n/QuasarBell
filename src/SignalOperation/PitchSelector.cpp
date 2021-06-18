#include "SignalOperation/PitchSelector.hpp"

#include "Audio/Signal.hpp"
#include "Audio/AudioRenderer.hpp"
#include "Audio/PcmData.hpp"

#include "App/AppInterface.hpp"

#include <array>

#include "imgui.h"

//--------------------------------------------------------------
PitchSelector::PitchSelector()
{
    makeInput("octave", BaseOperationDataType::Float);
    makeInput("semitone", BaseOperationDataType::Float);
    makeOutput("freq", BaseOperationDataType::Float);
    makeProperty("octave", BaseOperationDataType::Int, &octave);
    makeProperty("semitone", BaseOperationDataType::Int, &semitone);
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
bool PitchSelector::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    auto output1  = getOutput(0);
    qb::OperationData& data = visitor.data;
    data.type = output1->type;

    float oc = inputOrProperty(0, visitor, (float)octave);
    float se = inputOrProperty(1, visitor, (float)semitone);
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    constexpr int midi_first_octave = -1;
    float midiindex = (oc-midi_first_octave) * midi_semitones + se;
    data.fvec[0] = pitchToFreq(midiindex, midi_la440, midi_semitones);

    return true;
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

    if (ImGui::InputInt("octave", &octave)) dirty();
    if (ImGui::InputInt("semitone", &semitone)) dirty();
    if (ImGui::Button("Play"))
    {
        auto& sound = *SoundNode::getDefault();
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