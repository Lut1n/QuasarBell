#include "signal/operations/Waveform.hpp"

#include <cmath> // sin
#include <iostream>

#include "core/PseudoRand.hpp"

//--------------------------------------------------------------
Waveform::Waveform()
{
    initialize({},{DataType_Float});
    makeProperty({"period", DataType_Float, &period});
    makeProperty({"min", DataType_Float, &minVal});
    makeProperty({"max", DataType_Float, &maxVal});
    makeProperty({"type", DataType_Int, &type});
    makeProperty({"noise-seed", DataType_Int, &noiseSeed});
    makeProperty({"noise-samples", DataType_Int, &noiseSamples});
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
        return std::sin(t * pi2) >0.f ? 1.f : -1.f;
    }
    else if(type == Type::Noise)
    {
        while(t>1.f) t -= 1.f;
        return samples[(size_t)(t * samples.size())];
    }
    return 0.f;
}

//--------------------------------------------------------------
OperationData Waveform::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    
    data.type = output->type;
    data.count = output->count;

    float ampl = (maxVal-minVal);
    data.fvec[0] = (sampleFlip(t.t * period) + 1.0f) * 0.5f * ampl + minVal;
    return data;
}

void Waveform::computeNoise()
{
    if (_prev_seed != noiseSeed || _prev_samples != noiseSamples)
    {
        _prev_samples = noiseSamples;
        _prev_seed = noiseSeed;

        qb::noiseSeed(noiseSeed);
        samples.resize(noiseSamples);
        for(auto& s : samples) s = qb::noiseValue() * 2.f - 1.f;
    }
}