#include "signal/Signal.hpp"

#include <cmath> // sin

//--------------------------------------------------------------
AudioSettings AudioSettings::defaultSettings;

//--------------------------------------------------------------
SinSampler::SinSampler(float freq, float ampl)  : SignalSampler()
{
    _freq = freq;
    _ampl = ampl;
}

//--------------------------------------------------------------
float SinSampler::operator()(float t)
{
    return std::sin(PI2 * _freq * t) * _ampl;
}


//--------------------------------------------------------------
LinearGate::LinearGate(float tt_duration, float slope_duration)  : SignalSampler()
{
    _duration = tt_duration;
    _slopeTime = slope_duration;
}
    
//--------------------------------------------------------------
float LinearGate::operator()(float t)
{
    if(t < _slopeTime)
        return t/_slopeTime;
    else if(_duration-t < _slopeTime)
        return (_duration-t) / _slopeTime;
    return 1.0;
}