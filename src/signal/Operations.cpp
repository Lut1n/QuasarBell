#include "signal/Operations.hpp"

#include <cmath> // sin

//--------------------------------------------------------------
FloatInput::FloatInput()
{
    initialize({},{DataType_Float});
}
//--------------------------------------------------------------
void FloatInput::updateData()
{
}
//--------------------------------------------------------------
OperationData FloatInput::getData(size_t index, float t)
{
    OperationData data;
    // auto output  = getOutput(0);
    data.type = DataType_Float;
    data.count = 1;
    data.fvec[0] = value;
    return data;
}
//--------------------------------------------------------------
AddOperation::AddOperation()
{
    initialize({DataType_Undefined, DataType_Undefined},{DataType_Undefined});
}
//--------------------------------------------------------------
void AddOperation::updateData()
{
    auto* input1 = getInput(0);
    auto* input2 = getInput(1);
    if(input1->operation) input1->operation->updateData();
    if(input2->operation) input2->operation->updateData();
    auto* output  = getOutput(0);
    if (input1->type == input2->type)
        output->type = input1->type;
    else
        output->type = DataType_Error;
}
//--------------------------------------------------------------
OperationData AddOperation::getData(size_t index, float t)
{
    OperationData data;
    auto output  = getOutput(0);
    if(output->type != DataType_Error && output->type != DataType_Undefined)
    {
        OperationData a = getInputData(0);
        OperationData b = getInputData(1);
        data.type = output->type;
        data.count = output->count;
        if (data.type == DataType_Float)
            data.fvec[0] = a.fvec[0] + b.fvec[0];
        if (data.type == DataType_Int)
            data.ivec[0] = a.ivec[0] + b.ivec[0];
    }
    else
    {
        data.type = DataType_Undefined;
    }
    return data;
}
//--------------------------------------------------------------
MultOperation::MultOperation()
{
    initialize({DataType_Undefined, DataType_Undefined},{DataType_Undefined});
}
//--------------------------------------------------------------
void MultOperation::updateData()
{
    auto* input1 = getInput(0);
    auto* input2 = getInput(1);
    if(input1->operation) input1->operation->updateData();
    if(input2->operation) input2->operation->updateData();
    auto* output  = getOutput(0);
    if (input1->type == input2->type)
        output->type = input1->type;
    else
        output->type = DataType_Error;
}
//--------------------------------------------------------------
OperationData MultOperation::getData(size_t index, float t)
{
    OperationData data;
    auto output  = getOutput(0);
    if(output->type != DataType_Error && output->type != DataType_Undefined)
    {
        OperationData a = getInputData(0);
        OperationData b = getInputData(1);
        data.type = output->type;
        data.count = output->count;
        if (data.type == DataType_Float)
            data.fvec[0] = a.fvec[0] * b.fvec[0];
        if (data.type == DataType_Int)
            data.ivec[0] = a.ivec[0] * b.ivec[0];
    }
    else
    {
        data.type = DataType_Undefined;
    }
    return data;
}
//--------------------------------------------------------------
Ondulator::Ondulator()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
}
//--------------------------------------------------------------
void Ondulator::updateData()
{
    auto* input1 = getInput(0);
    auto* input2 = getInput(1);
    if(input1->operation) input1->operation->updateData();
    if(input2->operation) input2->operation->updateData();
    auto* output  = getOutput(0);
    if(input1->type == DataType_Float && input2->type == DataType_Float)
        output->type = DataType_Float;
    else
        output->type = DataType_Error;
}
//--------------------------------------------------------------
OperationData Ondulator::getData(size_t index, float t)
{
    OperationData data;
    auto output  = getOutput(0);
    if(output->type != DataType_Error && output->type != DataType_Undefined)
    {
        auto* input1 = getInput(0);
        auto* input2 = getInput(1);
        OperationData a = getInputData(0);
        OperationData b = getInputData(1);
        data.type = DataType_Float;
        float freq = 0.0;
        float ampl = 0.0;
        if(input1->operation) freq = input1->operation->getData(input1->index, t).fvec[0];
        if(input2->operation) ampl = input2->operation->getData(input2->index, t).fvec[0];
        data.fvec[0] = std::sin( t * 2.0 * 3.141592 * freq ) * ampl;
    }
    else
    {
        data.type = DataType_Undefined;
    }
    return data;
}

OutputOperation::OutputOperation()
{
    duration = 100.0;
    initialize({DataType_Float},{});
}
void OutputOperation::updateData()
{
    auto* input1 = getInput(0);
    if(input1->operation) input1->operation->updateData();
    
}
OperationData OutputOperation::getData(size_t index, float t)
{
    OperationData data;
    data.type = DataType_Undefined;
    return data;
}

float OutputOperation::sample(float t)
{
    float res = 0.0f;
    OperationData data = getData(0, t / duration);
    if(data.type == DataType_Float)
        res = data.fvec[0];
    return res;
}