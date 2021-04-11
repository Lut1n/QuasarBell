#ifndef QUASAR_BELL_SIGNAL_HPP
#define QUASAR_BELL_SIGNAL_HPP

#include <vector>

#include "Core/Math.hpp"

//--------------------------------------------------------------
#define La440 440.0f


//--------------------------------------------------------------
inline float halfStep(int n, float baseFreq = La440)
{
    return baseFreq * std::pow(2.0, (double)(n)/12.0);
}

//--------------------------------------------------------------
namespace Note
{
    static const float C4 = halfStep(-9, La440);
    // -8 : C4_# / D4_b
    static const float D4 = halfStep(-7, La440);
    // -6 : D4_# / E4_b
    static const float E4 = halfStep(-5, La440);
    static const float F4 = halfStep(-4, La440);
    // -3 : F4_# / G4_b
    static const float G4 = halfStep(-2, La440);
    // -2 : G4_# / A4_b
    static const float A4 = La440;
    // +1 : A4_# / B4_b
    static const float B4 = halfStep(2, La440);
    static const float C5 = halfStep(3, La440);
    
    static const float Do = C4;
    static const float Re = D4;
    static const float Mi = E4;
    static const float Fa = F4;
    static const float So = G4;
    static const float La = A4;
    static const float Si = B4;
    static const float Do2 = C5;
};

//--------------------------------------------------------------
class AudioSettings
{
public:
    enum SampleRate
    {
        SampleRate_8kHz = 8000,     // Not suitable for any serious recording task. May be adequate for some sfx
        SampleRate_11kHz = 11025,   // Very poor sound quality
        SampleRate_22kHz = 22050,   // Used by AM radio, very small mp3 players
        SampleRate_32kHz = 32000,   // Low quality (used by FM radio)
        SampleRate_44kHz = 44100,   // Med quality
        SampleRate_48kHz = 48000,   // High quality
    };
    
    enum Format
    {
        Format_Mono8,
        Format_Stereo8,
        Format_Mono16,
        Format_Stereo16
    };
    
public:
    int sampleRate = SampleRate_44kHz;
    Format sampleFormat = Format_Mono16;

    static AudioSettings defaultSettings;
};

//--------------------------------------------------------------
class SignalSampler
{
public:
    virtual float operator()(float t) = 0;
};

//--------------------------------------------------------------
class SinSampler : public SignalSampler
{
public:
    SinSampler(float freq = La440, float ampl = 1.0f);
    
    float operator()(float t) override;

private:
    float _freq;
    float _ampl;
    
    constexpr static float PI2 = 2.0 * 3.141592;
};

//--------------------------------------------------------------
class LinearGate : public SignalSampler
{
public:
    LinearGate(float tt_duration = 1.0, float slope_duration = 0.1);
    
    float operator()(float t) override;
    
private:
    float _duration;
    float _slopeTime;
};

//--------------------------------------------------------------
inline float freqToPitch(float freq, float la440Pitch = 69/*MIDI*/, float semitones = 12)
{
    return la440Pitch + semitones * qb::log2(freq / 440.0 );
}

//--------------------------------------------------------------
inline float pitchToFreq(float pitch, float la440Pitch = 69/*MIDI*/, float semitones = 12)
{
    return qb::pow2((pitch - la440Pitch) / semitones) * 440.0;
}

#endif // QUASAR_BELL_SIGNAL_HPP