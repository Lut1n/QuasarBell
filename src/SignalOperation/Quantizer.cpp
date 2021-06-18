#include "SignalOperation/Quantizer.hpp"

#include <iostream>

//--------------------------------------------------------------
Quantizer::Quantizer()
{
    makeInput("value", BaseOperationDataType::Float);
    makeInput("quantity", BaseOperationDataType::Int);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("quantity", BaseOperationDataType::Int, &quantity);
}
//--------------------------------------------------------------
bool Quantizer::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);

    int qu = (int) inputOrProperty(1, visitor, (float)quantity);
    data.type = output->type;
    data.fvec[0] = 0.0;

    if (qu > 0)
    {
        qb::PcmBuilderVisitor v2;
        int n = (int)(visitor.time.t * qu);
        float rounded = ((float)n+0.5f) / (float)qu;
        v2.time = visitor.time;
        v2.time.t = rounded;
        data.fvec[0] = inputOrProperty(0, visitor, 0.0);
    }

    return true;
}