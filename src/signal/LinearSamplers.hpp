#ifndef QUASAR_BELL_LINEAR_SAMPLER_HPP
#define QUASAR_BELL_LINEAR_SAMPLER_HPP

#include "signal/Signal.hpp"

//--------------------------------------------------------------
class LinearSampler : public SignalSampler
{
public:    
    float operator()(float t) override;
    
public:
    float value = 0.0f;
    float speed = 0.0f;
    float resetTime = 1.0f;
    float minVal = -1.0f;
    float maxVal = 1.0f;
    float minSpeed = -1.0f;
    float maxSpeed = 1.0f;
    float duration = 1.0f;
};


#endif // QUASAR_BELL_LINEAR_SAMPLER_HPP
