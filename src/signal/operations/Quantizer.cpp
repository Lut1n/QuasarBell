#include "signal/operations/Quantizer.hpp"

#include <iostream>

//--------------------------------------------------------------
Quantizer::Quantizer()
{
    initialize({DataType_Float,DataType_Int},{DataType_Float});
    makeProperty({"quantity", DataType_Int, &quantity});
}
//--------------------------------------------------------------
void Quantizer::validate()
{
}
//--------------------------------------------------------------
OperationData Quantizer::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    if (a.type == DataType_Float)
    {
        int qu = b.type == DataType_Int ? b.ivec[0] : quantity;
        data.type = output->type;
        data.count = output->count;
        data.fvec[0] = a.fvec[0];

        if (qu > 0)
        {
            int n = (int)(t.t * qu);
            float rounded = ((float)n+0.5f) / (float)qu;
            Time t2 = t;
            t2.t = rounded;
            a = sampleInput(0, t2);
            data.fvec[0] = a.fvec[0];
        }
    }
    else
    {
        data.type = DataType_Error;
    }

    return data;
}