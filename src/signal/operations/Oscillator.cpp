#include "signal/operations/Oscillator.hpp"

#include <cmath> // sin
#include <iostream>

//--------------------------------------------------------------
Oscillator::Oscillator()
{
    initialize({DataType_Float,DataType_Float,DataType_Float},{DataType_Float});
    makeProperty({"freq", DataType_Float, &freq});
    makeProperty({"ampl", DataType_Float, &ampl});
}
//--------------------------------------------------------------
void Oscillator::validate()
{
    phases.clear();
}
//--------------------------------------------------------------
OperationData Oscillator::sample(size_t index, const Time& t)
{
    float& phase = phases[t.dstOp];

    t.dstOp = this;
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