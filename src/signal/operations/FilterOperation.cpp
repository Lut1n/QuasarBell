#include "signal/operations/FilterOperation.hpp"

//--------------------------------------------------------------
FreqFilter::FreqFilter()
{
    initialize({DataType_Float,DataType_Float,DataType_Float},{DataType_Float,DataType_Float}, {
            {"offset",DataType_Float},
            {"length",DataType_Float},
            {"minGain",DataType_Float},
            {"maxGain",DataType_Float},
            {"factor",DataType_Float}});
}
//--------------------------------------------------------------
void FreqFilter::validate()
{
}
//--------------------------------------------------------------
OperationData FreqFilter::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output1  = getOutput(0);
    auto output2  = getOutput(1);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float freq = a.type == DataType_Float ? a.fvec[0] : 440.0;
    float ampl = b.type == DataType_Float ? b.fvec[0] : 1.0;
    float fct = c.type == DataType_Float ? c.fvec[0] : factor;
    
    auto min = [](float f1, float f2) {return f1<f2?f1:f2;};

    float gain = 0.0;
    if (length > 0.0)
        gain = 1.0 - min(1.0, std::abs(freq - offset) / length);

    if (index == 1)
    {
        data.type = output2->type;
        data.count = output2->count;
        float res = ampl + fct * (minGain + gain * (maxGain-minGain));
        data.fvec[0] = res > 1.0 ? 1.0 : (res < 0.0 ? 0.0 : res);
    }
    else
    {
        data.type = output1->type;
        data.count = output1->count;
        data.fvec[0] = freq;
    }
    return data;
}

void FreqFilter::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = offset;
    else if (i==1)
        value = length;
    else if (i==2)
        value = minGain;
    else if (i==3)
        value = maxGain;
    else if (i==4)
        value = factor;
}
void FreqFilter::setProperty(size_t i, float value)
{
    if (i==0)
        offset = value;
    else if (i==1)
        length = value;
    else if (i==2)
        minGain = value;
    else if (i==3)
        maxGain = value;
    else if (i==4)
        factor = value;
}