#include "Modulator.hpp"

#include <cmath> // sin
#include <limits>

#include "signal/Signal.hpp"

//--------------------------------------------------------------
qb::Pcm16 Modulator::silent(float duration)
{
    auto& settings = AudioSettings::defaultSettings;
    
    qb::Pcm16 output;
    output.resize(duration * settings.sampleRate);
    for(unsigned i=0;i<output.count();++i) output.set(i, 0.0);
    return output;
}

//--------------------------------------------------------------
qb::Pcm16 Modulator::generate(float freq, float duration, float ampl, float slope_time, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    SinSampler instrument(30.0f, 1.0);
    LinearGate gate(duration, slope_time);
    
    qb::Pcm16 output;
    output.resize(duration * settings.sampleRate);
    
    for(unsigned i=0;i<output.count();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        output.set(i, signal(oft_t + t) * /*instrument(t) **/ gate(t));
    }
    return output;
}
//--------------------------------------------------------------
qb::Pcm16 Modulator::attack(float freq, float duration, float ampl, float ampl2, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    
    qb::Pcm16 output;
    output.resize(duration * settings.sampleRate);
    
    float mid = 0.5 * duration;
    
    for(unsigned i=0;i<output.count();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        float gate = t<mid ? (t/mid) : (1.0-(t-mid/mid))*ampl2;
        output.set(i, signal(oft_t + t) * gate);
    }
    return output;
}
//--------------------------------------------------------------
qb::Pcm16 Modulator::release(float freq, float duration, float ampl, float oft_t)
{
    auto& settings = AudioSettings::defaultSettings;
    
    SinSampler signal(freq, ampl);
    
    qb::Pcm16 output;
    output.resize(duration * settings.sampleRate);
    
    for(unsigned i=0;i<output.count();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        output.set(i, signal(oft_t + t) * (1.0-t/duration));
    }
    return output;
}
//--------------------------------------------------------------
qb::Pcm16 Modulator::mix(const std::list<qb::Pcm16>& inputStream, float duration)
{
    float ratio = 1.0f / (float)inputStream.size();
    
    qb::Pcm16 output = Modulator::silent(duration);
    for(size_t i=0; i<output.count(); ++i)
    {
        float res = 0.0;
        for(auto& pcm : inputStream) res += pcm.get(i);
        output.set(i, res * ratio);
    }
    return output;
}