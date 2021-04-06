#include "signal/operations/Oscillator.hpp"

#include <cmath> // sin
#include <iostream>

//--------------------------------------------------------------
Oscillator::Oscillator()
{
    initialize({DataType_Float,DataType_Float,DataType_Int},{DataType_Float});
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
    OperationData c = sampleInput(2, t);

    float fe = a.type == DataType_Float ? a.fvec[0] : freq;
    float am = b.type == DataType_Float ? b.fvec[0] : ampl;
    float wf = c.type == DataType_Int ? c.fvec[0] : waveform;

    data.type = output->type;
    data.count = output->count;
    phase += t.elapsed * fe;
    data.fvec[0] = std::sin(phase * 2.0f * 3.141592f) * am;
    return data;
}

size_t Oscillator::getPropertyCount() const
{
    return 3;
}
std::string Oscillator::getPropertyName(size_t i) const
{
    if (i==0)
        return "freq";
    if (i==1)
        return "ampl";
    if (i==2)
        return "waveform";
    return "None";
}
OperationDataType Oscillator::getPropertyType(size_t i) const
{
    if (i==0)
        return DataType_Float;
    if (i==1)
        return DataType_Float;
    if (i==2)
        return DataType_Int;
    return DataType_Error;
}
void Oscillator::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = freq;
    else if (i==1)
        value = ampl;
    else if (i==2)
        value = waveform;
}
void Oscillator::setProperty(size_t i, float value)
{
    if (i==0)
        freq = value;
    else if (i==1)
        ampl = value;
    else if (i==2)
        waveform = value;
}