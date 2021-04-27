#include "signal/operations/StepOperation.hpp"

#include <iostream>

//--------------------------------------------------------------
StepOperation::StepOperation()
{
    initialize({DataType_Float,DataType_Float,DataType_Float,DataType_Float},{DataType_Float});
    makeProperty({"edge", DataType_Float, &edge});
    makeProperty({"then", DataType_Float, &thenValue});
    makeProperty({"else", DataType_Float, &elseValue});
}
//--------------------------------------------------------------
void StepOperation::validate()
{
}
//--------------------------------------------------------------
OperationData StepOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);
    OperationData d = sampleInput(3, t);

    float test = a.type == DataType_Float ? a.fvec[0] : 0.0f;
    float ed = b.type == DataType_Float ? b.fvec[0] : edge;
    float tv = c.type == DataType_Float ? c.fvec[0] : thenValue;
    float ev = d.type == DataType_Float ? d.fvec[0] : elseValue;

    data.type = output->type;
    data.count = output->count;

    data.fvec[0] = test > ed ? tv : ev;
    return data;
}