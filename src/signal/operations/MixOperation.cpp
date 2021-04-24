#include "signal/operations/MixOperation.hpp"

//--------------------------------------------------------------
MixOperation::MixOperation()
{
    initialize({DataType_Float,DataType_Float,DataType_Float},{DataType_Float}, {
            {"input1",DataType_Float},
            {"input2",DataType_Float},
            {"delta",DataType_Float}});
}
//--------------------------------------------------------------
void MixOperation::validate()
{
}
//--------------------------------------------------------------
OperationData MixOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float in1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float in2 = b.type == DataType_Float ? b.fvec[0] : input2;
    float dt = c.type == DataType_Float ? c.fvec[0] : delta;

    data.type = output->type;
    data.count = output->count;

    data.fvec[0] = in1 * (1.0-dt) + in2 * dt;
    return data;
}

void MixOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
    else if (i==2)
        value = delta;
}
void MixOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
    else if (i==2)
        delta = value;
}