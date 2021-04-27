#include "signal/operations/EnvelopOperation.hpp"

//--------------------------------------------------------------
EnvelopOperation::EnvelopOperation()
{
    initialize({}, {DataType_Float});
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
        rt = sustain * (1.0f-(t-release)/(1.0-release));
    }
    else
    {
        rt = 0.0f;
    }
    rt = rt > 1.0 ? 1.0 : (rt<-0.0 ? 0.0 : rt);
    return rt;
}