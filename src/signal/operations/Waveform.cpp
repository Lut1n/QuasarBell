#include "signal/operations/Waveform.hpp"

#include <cmath> // sin
#include <iostream>

#include "Core/PseudoRand.hpp"

//--------------------------------------------------------------
Waveform::Waveform()
{
    initialize({},{DataType_Float}, {
            {"period",DataType_Float},
            {"min",DataType_Float},
            {"max",DataType_Float},
            {"type",DataType_Float},
            {"noise-seed",DataType_Float},
            {"noise-samples",DataType_Float}});
}
//--------------------------------------------------------------
void Waveform::validate()
{
    if (type == Type::Noise)
    {
        computeNoise();
    }
}

//--------------------------------------------------------------
float Waveform::sampleFlip(float t)
{
    constexpr float pi2 = 2.0f * 3.141592f;
    if(type == Type::Sin)
    {
        return std::sin(t * pi2);
    }
    else if(type == Type::Saw)
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
    else if(type == Type::Square)
    {
        return std::sin(t * pi2) >0.0f ? 1.0 : -1.0;
    }
    else if(type == Type::Noise)
    {
        while(t>1.0) t -= 1.0;
        return samples[(size_t)(t * samples.size())];
    }
    return 0.0;
}

//--------------------------------------------------------------
OperationData Waveform::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    
    data.type = output->type;
    data.count = output->count;

    float ampl = (maxVal-minVal);
    data.fvec[0] = (sampleFlip(t.t * period) + 1.0) * 0.5 * ampl + minVal;
    return data;
}

void Waveform::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = period;
    else if (i==1)
        value = minVal;
    else if (i==2)
        value = maxVal;
    else if (i==3)
        value = type;
    else if (i==4)
        value = noiseSeed;
    else if (i==5)
        value = noiseSamples;
}
void Waveform::setProperty(size_t i, float value)
{
    if (i==0)
        period = value;
    else if (i==1)
        minVal = value;
    else if (i==2)
        maxVal = value;
    else if (i==3)
        type = value;
    else if (i==4)
        noiseSeed = value;
    else if (i==5)
        noiseSamples = value;
}

void Waveform::computeNoise()
{
    if (_prev_seed != noiseSeed || _prev_samples != noiseSamples)
    {
        _prev_samples = noiseSamples;
        _prev_seed = noiseSeed;

        qb::noiseSeed(noiseSeed);
        samples.resize(noiseSamples);
        for(auto& s : samples) s = qb::noiseValue() * 2.0 - 1.0;
    }
}