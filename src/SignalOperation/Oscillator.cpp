#include "SignalOperation/Oscillator.hpp"

#include <cmath> // sin
#include <iostream>

//--------------------------------------------------------------
Oscillator::Oscillator()
{
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("ampl", BaseOperationDataType::Float);
    makeInput("waveform", BaseOperationDataType::Float);
    makeOutput("signal", BaseOperationDataType::Float);
    makeProperty("freq", BaseOperationDataType::Float, &freq);
    makeProperty("ampl", BaseOperationDataType::Float, &ampl);
}
//--------------------------------------------------------------
void Oscillator::startSampling()
{
    phases.clear();
}
//--------------------------------------------------------------
bool Oscillator::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    float& phase = visitor.phase; // phases[t.dstOp];

    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    
    float fe = inputOrProperty(0, visitor, freq);
    float am = inputOrProperty(1, visitor, ampl);

    phase += visitor.time.elapsed * fe;

    qb::PcmBuilderVisitor tWaveform;
    tWaveform.phase = visitor.phase;
    tWaveform.time = visitor.time;
    tWaveform.time.t = phase;
    while(tWaveform.time.t > 1.0) tWaveform.time.t -= 1.0;

    data.type = output->type;

    float dfltVal = std::sin(phase * 2.0f * 3.141592f);
    data.fvec[0] = inputOrProperty(2, tWaveform, dfltVal);
    data.fvec[0] *= am;
    
    return true;
}