#include "SignalOperation/StepOperation.hpp"

#include <iostream>

//--------------------------------------------------------------
StepOperation::StepOperation()
{
    makeInput("value", BaseOperationDataType::Float);
    makeInput("edge", BaseOperationDataType::Float);
    makeInput("then", BaseOperationDataType::Float);
    makeInput("else", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("edge", BaseOperationDataType::Float, &edge);
    makeProperty("then", BaseOperationDataType::Float, &thenValue);
    makeProperty("else", BaseOperationDataType::Float, &elseValue);
}
//--------------------------------------------------------------
bool StepOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float test = inputOrProperty(0, visitor, 0.0);
    float ed = inputOrProperty(1, visitor, edge);
    float tv = inputOrProperty(2, visitor, thenValue);
    float ev = inputOrProperty(3, visitor, elseValue);

    data.type = output->type;
    data.fvec[0] = test > ed ? tv : ev;
    return true;
}