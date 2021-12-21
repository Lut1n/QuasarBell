#include "SignalOperation/SignalOperations.hpp"

#include <iostream>
#include <string>

#include "imgui.h"
#include "App/AppInterface.hpp"

#include "Json/Json.hpp"

#include "Core/PseudoRand.hpp"
#include "SignalOperation/PcmBuilder.hpp"


//--------------------------------------------------------------
template<typename Ty>
Ty inputOrAttribute(const Time& time, OperationInfo* input, Ty attr)
{
    if (input && input->attributes)
    {
        auto op = input->operation;
        return op->sample(time, input->attributes, input->inputs);
    }
    else
        return attr;
}

//--------------------------------------------------------------
float EnvelopOperation::sample(const Time& time,EnvelopData* attributes, std::vector<OperationInfo*>& inputs)
{
    float t = time.advance;
    float attack = attributes->attack;
    float decay = attributes->decay;
    float release = attributes->release;
    float sustain = attributes->sustain;

    float rt = 0.0f;
    if(t < attack)
    {
        rt = t / attack;
    }
    else if(t-attack < decay)
    {
        rt = 1.0f - (1.0f - sustain) * (t-attack) / decay;
    }
    else if(t < release)
    {
        rt = sustain;
    }
    else if(t < 1.0)
    {
        rt = sustain * (1.f-(t-release)/(1.f-release));
    }
    else
    {
        rt = 0.0f;
    }
    rt = rt > 1.f ? 1.f : (rt<-0.f ? 0.f : rt);
    return rt;
}
//--------------------------------------------------------------
float FreqFilter::sample(const Time& time,FreqFilterData* attributes, std::vector<OperationInfo*>& inputs)
{
    float freq = inputOrAttribute(time, inputs[0], 440.0);
    float ampl = inputOrAttribute(time, inputs[1], 1.0);
    float fct = inputOrAttribute(time, inputs[2], attributes->factor);

    auto min = [](float f1, float f2) {return f1<f2?f1:f2;};

    float gain = 0.f;
    if (attributes->length > 0.0)
        gain = 1.f - min(1.f, std::abs(freq - attributes->offset) / attributes->length);

    float res = ampl + fct * (attributes->minGain + gain * (attributes->maxGain-attributes->minGain));
    return res > 1.f ? 1.f : (res < 0.f ? 0.f : res);
}
//--------------------------------------------------------------
float Harmonics::sample(const Time& time,HarmonicsData* attributes, std::vector<OperationInfo*>& inputs)
{
    float ret = 0.0f;
    for(auto f : attributes->freqs) ret += std::sin( time.advance * f.first * 2.f * 3.141592f ) * f.second;
    return ret;
}
//--------------------------------------------------------------
float KeySampler::sample(const Time& time,KeySamplerData* attributes, std::vector<OperationInfo*>& inputs)
{
    int index1 = -1;
    int index2 = -1;
    auto& keys = attributes->keys;
    float t = time.advance;

    for(size_t i=0; i<keys.size(); ++i)
    {
        int j = (int)(keys.size()-1-i);
        if(keys[i].first <= t) index1 = (int)i;
        if(keys[j].first >= t) index2 = (int)j;
    }

    if(index1 == -1 && index2 == -1)
        return 0.0;
    else if(index1 == -1)
        return keys[index2].second;
    else if(index2 == -1)
        return keys[index1].second;
    else if(index1 == index2)
        return keys[index1].second;
    else
    {
        float x = (t-keys[index1].first) / (keys[index2].first-keys[index1].first);
        return keys[index1].second + interpolate(x, attributes) * (keys[index2].second-keys[index1].second);
    }
}
//--------------------------------------------------------------
float KeySampler::interpolate(float x, KeySamplerData* props)
{
    if (props->interpo == Interpo::Flat)
        return 0;
    else if (props->interpo == Interpo::Linear)
        return x;
    else if (props->interpo == Interpo::Cubic)
        return x * x * (3.f - 2.f * x);
    return x;
}
//--------------------------------------------------------------
float MixOperation::sample(const Time& time,MixData* attributes, std::vector<OperationInfo*>& inputs)
{
    float in1 = inputOrAttribute(time, inputs[0], attributes->input1);
    float in2 = inputOrAttribute(time, inputs[1], attributes->input2);
    float dt = inputOrAttribute(time, inputs[2], attributes->delta);
    return in1 * (1.f-dt) + in2 * dt;
}
//--------------------------------------------------------------
float Oscillator::sample(const Time& time, OscillatorData* attributes, std::vector<OperationInfo*>& inputs)
{
    static float phase = 0.0f;
    if (time.advance == 0.0f)
        phase = 0.0f;

    float fe = inputOrAttribute(time, inputs[0], attributes->freq);
    float am = inputOrAttribute(time, inputs[1], attributes->ampl);

    phase += time.timestep * fe;
    while(phase > 1.0f) phase -= 1.0f;

    Time time2;
    time2.duration = 1.0f;
    time2.elapsed = phase;
    time2.advance = phase;

    float dfltVal = std::sin(phase * 2.0f * 3.141592f);
    float wave = inputOrAttribute(time2, inputs[2], dfltVal);
    return wave * am;
}

//--------------------------------------------------------------
int PitchSelector::getMidiIndex(PitchData* props) const
{
    constexpr int midi_first_octave = -1;
    constexpr int midi_semitones = 12;
    return (props->octave-midi_first_octave) * midi_semitones + props->semitone;
}
//--------------------------------------------------------------
float PitchSelector::getFreq(PitchData* props) const
{
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    return pitchToFreq((float)getMidiIndex(props), midi_la440, midi_semitones);
}
//--------------------------------------------------------------
float PitchSelector::sample(const Time& time,PitchData* attributes, std::vector<OperationInfo*>& inputs)
{
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    constexpr int midi_first_octave = -1;

    float oc = inputOrAttribute(time, inputs[0], (float)attributes->octave);
    float se = inputOrAttribute(time, inputs[1], (float)attributes->semitone);
    float midiindex = (oc-midi_first_octave) * midi_semitones + se;
    return pitchToFreq(midiindex, midi_la440, midi_semitones);
}

//--------------------------------------------------------------
float Quantizer::sample(const Time& time,QuantizerData* attributes, std::vector<OperationInfo*>& inputs)
{
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    constexpr int midi_first_octave = -1;

    int qu = (int) inputOrAttribute(time, inputs[1], (int)attributes->quantity);
    if (qu <= 0) qu = 1;

    int n = (int)(time.advance * qu);
    Time time2 = time;
    time2.advance = ((float)n+0.5f) / (float)qu;
    time2.elapsed = time2.advance * time2.duration;
    return inputOrAttribute(time2, inputs[0], 0.0f);
}

//--------------------------------------------------------------
float StepOperation::sample(const Time& time,StepData* attributes, std::vector<OperationInfo*>& inputs)
{
    float test = inputOrAttribute(time, inputs[0], 0.0f);
    float ed = inputOrAttribute(time, inputs[1], attributes->edge);
    float tv = inputOrAttribute(time, inputs[2], attributes->thenValue);
    float ev = inputOrAttribute(time, inputs[3], attributes->elseValue);
    return test > ed ? tv : ev;
}
//--------------------------------------------------------------
float Waveform::sampleClip(float t, WaveformData* attributes)
{
    auto type = (WaveformData::WaveType)attributes->type;
    auto& noiseSamples = attributes->samples;

    constexpr float pi2 = 2.0f * 3.141592f;
    if(type == WaveformData::WaveType::Sin)
    {
        return std::sin(t * pi2);
    }
    else if(type == WaveformData::WaveType::Saw)
    {
        while(t>1.0) t -= 1.0;
        if(t<0.25f)
        {
            return t/0.25f;
        }
        else if(t<0.50f)
        {
            return 1.0f - (t-0.25f)/0.25f;
        }
        else if(t<0.75f)
        {
            return -(t-0.50f)/0.25f;
        }
        else
        {
            return (t-0.75f)/0.25f - 1.0f;
        }
    }
    else if(type == WaveformData::WaveType::Square)
    {
        return std::sin(t * pi2) >0.f ? 1.f : -1.f;
    }
    else if(type == WaveformData::WaveType::Noise)
    {
        return noiseSamples[t * noiseSamples.size()];
    }
    return 0.f;
}

//--------------------------------------------------------------
float Waveform::sample(const Time& time,WaveformData* attributes, std::vector<OperationInfo*>& inputs)
{
    attributes->computeNoise();
    float ampl = (attributes->maxVal-attributes->minVal);

    float s = sampleClip(time.advance * attributes->period, attributes);
    return (s + 1.0f) * 0.5f * ampl + attributes->minVal;
}

//--------------------------------------------------------------
float FloatInput::sample(const Time& time,FloatData* attributes, std::vector<OperationInfo*>& inputs)
{
    return attributes->value;
}
//--------------------------------------------------------------
float NoiseInput::sample(const Time& time,NoiseData* attributes, std::vector<OperationInfo*>& inputs)
{
    return qb::noiseValue() * 2.f - 1.f;
}
//--------------------------------------------------------------
float Repeater::sample(const Time& time,RepeaterData* attributes, std::vector<OperationInfo*>& inputs)
{
    float qu = inputOrAttribute(time, inputs[1], attributes->count);
    if (qu==0.0f) qu=1.0f;

    Time time2 = time;
    time2.advance = time.advance * qu;
    while(time2.advance > 1.0f) time2.advance -= 1.0f;
    time2.elapsed = time2.advance * time.duration;

    return inputOrAttribute(time2, inputs[0], 0.0f);
}
//--------------------------------------------------------------
float CubicSampler::sample(const Time& time,CubicSamplerData* attributes, std::vector<OperationInfo*>& inputs)
{
    if(attributes->reset < 0.0f) attributes->reset = 0.0f;

    float va = inputOrAttribute(time, inputs[0], attributes->value);
    float sp = inputOrAttribute(time, inputs[1], attributes->speed);
    float ac = inputOrAttribute(time, inputs[2], attributes->acc);
    float je = inputOrAttribute(time, inputs[3], attributes->jerk);
    float rs = inputOrAttribute(time, inputs[4], attributes->reset);

    auto sampler = [va,sp,ac,je,rs](float t)
    {
        if(rs != 0.0f)
        {
            while(t>=rs) t -= rs;
        }
        return va + (sp + (ac + je * t) * t) * t;
    };

    return sampler(time.advance);
}
//--------------------------------------------------------------
float PolynomialSampler::sample(const Time& time,PolynomialSamplerData* attributes, std::vector<OperationInfo*>& inputs)
{
    if(attributes->reset < 0.0f) attributes->reset = 0.0f;

    float x = time.advance;
    if(attributes->reset > 0.0)
    {
        while(x>=attributes->reset) x -= attributes->reset;
    }
    float res = 0.0;
    float ve = 1.0;
    for(auto coef : attributes->coefs)
    {
        res += coef * ve;
        ve *= x;
    }
    return res;
}

//--------------------------------------------------------------
#define BASIC_OPERATION(PREFIX, SYMBOL)\
float PREFIX##Operation::sample(const Time& time,PREFIX##Data* attributes, std::vector<OperationInfo*>& inputs) \
{ \
    float in1 = inputOrAttribute(time, inputs[0], attributes->input1);\
    float in2 = inputOrAttribute(time, inputs[1], attributes->input2);\
    return in1 SYMBOL in2;\
}\

BASIC_OPERATION(Add,+)
BASIC_OPERATION(Sub,-)
BASIC_OPERATION(Mult,*)
BASIC_OPERATION(Div,/)

//--------------------------------------------------------------
float ClampOperation::sample(const Time& time,ClampData* attributes, std::vector<OperationInfo*>& inputs)
{
    float in1 = inputOrAttribute(time, inputs[0], attributes->input1);
    float e1 = inputOrAttribute(time, inputs[1], attributes->minVal);
    float e2 = inputOrAttribute(time, inputs[2], attributes->maxVal);
    return in1>e2 ? e2 : (in1<e1 ? e1 : in1);
}
//--------------------------------------------------------------
float AbsOperation::sample(const Time& time,AbsData* attributes, std::vector<OperationInfo*>& inputs)
{
    float in1 = inputOrAttribute(time, inputs[0], 0.0f);
    return in1 < 0.0f ? -in1 : in1;
}
//--------------------------------------------------------------
float TimeScale::sample(const Time& time,TimeScaleData* attributes, std::vector<OperationInfo*>& inputs)
{
    Time time2;
    time2 = time;
    time2.elapsed = time.elapsed * attributes->scale - attributes->delay;
    time2.advance = time2.elapsed / time2.duration;
    return inputOrAttribute(time2, inputs[0], 0.0f);
}
//--------------------------------------------------------------
float OutputOperation::sample(const Time& time,OutputData* attributes, std::vector<OperationInfo*>& inputs)
{
    return inputOrAttribute(time, inputs[0], 0.0f);
}
