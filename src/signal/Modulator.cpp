#include "Modulator.hpp"

#include <cmath> // sin
#include <limits>

#include "signal/Signal.hpp"

//--------------------------------------------------------------
PcmData Modulator::silent(float duration)
{
    auto& settings = AudioSettings::defaultSettings;
    
    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    for(unsigned i=0;i<output.samples.size();++i)
    {
        output.samples[i] = 0.0;
    }
    return output;
}

//--------------------------------------------------------------
PcmData Modulator::generate(float freq, float duration, float ampl, float slope_time, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    SinSampler instrument(30.0f, 1.0);
    LinearGate gate(duration, slope_time);
    
    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    short quantizer = std::numeric_limits<short>::max();
    
    for(unsigned i=0;i<output.samples.size();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        output.samples[i] = signal(oft_t + t) * /*instrument(t) **/ gate(t) * quantizer;
    }
    return output;
}
//--------------------------------------------------------------
PcmData Modulator::attack(float freq, float duration, float ampl, float ampl2, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    
    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    short quantizer = std::numeric_limits<short>::max();
    
    float mid = 0.5 * duration;
    
    for(unsigned i=0;i<output.samples.size();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        float gate = t<mid ? (t/mid) : (1.0-(t-mid/mid))*ampl2;
        output.samples[i] = signal(oft_t + t) * gate * quantizer;
    }
    return output;
}
//--------------------------------------------------------------
PcmData Modulator::release(float freq, float duration, float ampl, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    
    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    short quantizer = std::numeric_limits<short>::max();
    
    for(unsigned i=0;i<output.samples.size();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        output.samples[i] = signal(oft_t + t) * (1.0-t/duration) * quantizer;
    }
    return output;
}
//--------------------------------------------------------------
PcmData Modulator::mix(const std::list<PcmData>& inputStream, float duration)
{
    float ratio = 1.0f / (float)inputStream.size();
    
    PcmData output = Modulator::silent(duration);
    for(auto& pcm : inputStream)
    {
        output = output + (pcm * ratio);
    }
    return output;
}