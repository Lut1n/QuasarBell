#include "LinearSamplers.hpp"

//--------------------------------------------------------------
inline float clamp(float val, float v_min, float v_max)
{
    return (val<v_min)? v_min : (val>v_max)? v_max : val;
}
//--------------------------------------------------------------
float LinearSampler::operator()(float t)
{
    t /= duration;
    if(resetTime == 0.0f)
    {
        t = 0.0f;
    }
    else
    {
        while(t>=resetTime) t -= resetTime;
    }
    
    float val = value + speed * t;
    return clamp(val, minVal, maxVal);
}