#include "SignalOperation/EnvelopOperation.hpp"

//--------------------------------------------------------------
EnvelopOperation::EnvelopOperation()
{
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("attack", BaseOperationDataType::Float, &attack);
    makeProperty("decay", BaseOperationDataType::Float, &decay);
    makeProperty("sustain", BaseOperationDataType::Float, &sustain);
    makeProperty("release", BaseOperationDataType::Float, &release);
}
//--------------------------------------------------------------
bool EnvelopOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // visitor.dstOp = this;
    auto output  = getOutput(0);
    qb::OperationData& data = visitor.data;
    data.type = output->type;
    data.fvec[0] = sampleADSR(visitor.time.t);
    return true;
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