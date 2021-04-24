#include "signal/operations/EnvelopOperation.hpp"

//--------------------------------------------------------------
EnvelopOperation::EnvelopOperation()
{
    initialize({}, {DataType_Float}, {
            {"attack",DataType_Float},
            {"decay",DataType_Float},
            {"sustain",DataType_Float},
            {"release",DataType_Float}});
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

void EnvelopOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = attack;
    else if (i==1)
        value = decay;
    else if (i==2)
        value = sustain;
    else if (i==3)
        value = release;
}
void EnvelopOperation::setProperty(size_t i, float value)
{
    if (i==0)
        attack = value;
    else if (i==1)
        decay = value;
    else if (i==2)
        sustain = value;
    else if (i==3)
        release = value;
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