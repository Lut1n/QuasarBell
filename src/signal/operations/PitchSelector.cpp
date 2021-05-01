#include "signal/operations/PitchSelector.hpp"

#include "signal/Signal.hpp"

#include <array>

//--------------------------------------------------------------
PitchSelector::PitchSelector()
{
    initialize({DataType_Float,DataType_Float},{DataType_Float});
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