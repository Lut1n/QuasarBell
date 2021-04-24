#include "signal/operations/Oscillator.hpp"

#include <cmath> // sin
#include <iostream>

//--------------------------------------------------------------
Oscillator::Oscillator()
{
    initialize({DataType_Float,DataType_Float,DataType_Float},{DataType_Float}, {
            {"freq",DataType_Float},
            {"ampl",DataType_Float}});
}
//--------------------------------------------------------------
void Oscillator::validate()
{
    phase = 0.0;
}
//--------------------------------------------------------------
OperationData Oscillator::sample(size_t index, const Time& t)
{

    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float fe = a.type == DataType_Float ? a.fvec[0] : freq;
    float am = b.type == DataType_Float ? b.fvec[0] : ampl;

    phase += t.elapsed * fe;

    Time tWaveform = t;
    tWaveform.t = phase;
    while(tWaveform.t > 1.0) tWaveform.t -= 1.0;
    OperationData c = sampleInput(2, tWaveform);

    data.type = output->type;
    data.count = output->count;
    
    if (c.type == DataType_Float)
        data.fvec[0] = c.fvec[0] * am;
    else
        data.fvec[0] = std::sin(phase * 2.0f * 3.141592f) * am;
    return data;
}

void Oscillator::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = freq;
    else if (i==1)
        value = ampl;
}
void Oscillator::setProperty(size_t i, float value)
{
    if (i==0)
        freq = value;
    else if (i==1)
        ampl = value;
}