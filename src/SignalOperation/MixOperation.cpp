#include "SignalOperation/MixOperation.hpp"

//--------------------------------------------------------------
MixOperation::MixOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("input2", BaseOperationDataType::Float);
    makeInput("delta", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1",BaseOperationDataType::Float, &input1);
    makeProperty("input2",BaseOperationDataType::Float, &input2);
    makeProperty("delta",BaseOperationDataType::Float, &delta);
}
//--------------------------------------------------------------
bool MixOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float in1 = inputOrProperty(0, visitor, input1);
    float in2 = inputOrProperty(1, visitor, input2);
    float dt = inputOrProperty(2, visitor, delta);

    data.type = output->type;

    data.fvec[0] = in1 * (1.f-dt) + in2 * dt;
    return true;
}