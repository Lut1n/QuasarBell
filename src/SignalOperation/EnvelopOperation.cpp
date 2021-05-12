#include "SignalOperation/EnvelopOperation.hpp"

//--------------------------------------------------------------
EnvelopOperation::EnvelopOperation()
{
    // initialize({}, {DataType_Float});
    makeOutput("value", DataType_Float);
    makeProperty({"attack", DataType_Float, &attack});
    makeProperty({"decay", DataType_Float, &decay});
    makeProperty({"sustain", DataType_Float, &sustain});
    makeProperty({"release", DataType_Float, &release});
}
//--------------------------------------------------------------
void EnvelopOperation::validate()
{
}
//--------------------------------------------------------------
OperationData EnvelopOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = sampleADSR(t.t);
    return data;
}

float EnvelopOperation::sampleADSR(float t)
{
    float rt = 0.0f;
    if(t < attack)
    {
        rt = t / attack;
    }
    else if(t-attack < decay)
    {
        rt = 1.0f - (1.0f - sustain) * (t-attack) / decay;
    }
    else if(t < release)
    {
        rt = sustain;
    }
    else if(t < 1.0)
    {
        rt = sustain * (1.f-(t-release)/(1.f-release));
    }
    else
    {
        rt = 0.0f;
    }
    rt = rt > 1.f ? 1.f : (rt<-0.f ? 0.f : rt);
    return rt;
}