#include "signal/operations/Operations.hpp"

#include <iostream>

//--------------------------------------------------------------
FloatInput::FloatInput()
{
    initialize({},{DataType_Float});
}
//--------------------------------------------------------------
void FloatInput::validate()
{
}
//--------------------------------------------------------------
OperationData FloatInput::sample(size_t index, const Time& t)
{
    OperationData data;
    data.type = DataType_Float;
    data.count = 1;
    data.fvec[0] = value;
    return data;
}

size_t FloatInput::getPropertyCount() const
{
    return 1;
}
std::string FloatInput::getPropertyName(size_t i) const
{
    if (i==0)
        return "value";
    return "None";
}
OperationDataType FloatInput::getPropertyType(size_t i) const
{
    return DataType_Float;
}
void FloatInput::getProperty(size_t i, float& value) const
{
    value = this->value;
}
void FloatInput::setProperty(size_t i, float value)
{
    this->value = value;
}

//--------------------------------------------------------------
LinearInput::LinearInput()
{
    initialize({DataType_Float,DataType_Float,DataType_Float},{DataType_Float});
}
//--------------------------------------------------------------
void LinearInput::validate()
{
}
//--------------------------------------------------------------
OperationData LinearInput::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float va = a.type != DataType_Error ? a.fvec[0] : value;
    float sp = b.type != DataType_Error ? b.fvec[0] : speed;
    float ac = c.type != DataType_Error ? c.fvec[0] : acc;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = va + t.t*sp + t.t*t.t*ac;
    return data;
}

size_t LinearInput::getPropertyCount() const
{
    return 3;
}
std::string LinearInput::getPropertyName(size_t i) const
{
    if (i==0)
        return "value";
    if (i==1)
        return "speed";
    if (i==2)
        return "acc";
    return "None";
}
OperationDataType LinearInput::getPropertyType(size_t i) const
{
    return DataType_Float;
}
void LinearInput::getProperty(size_t i, float& value) const
{
    if (i == 0)
        value = this->value;
    else if (i == 1)
        value = this->speed;
    else if (i == 2)
        value = this->acc;
}
void LinearInput::setProperty(size_t i, float value)
{
    if (i == 0)
        this->value = value;
    else if (i == 1)
        this->speed = value;
    else if (i == 2)
        this->acc = value;
}

//--------------------------------------------------------------
AddOperation::AddOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
}
//--------------------------------------------------------------
void AddOperation::validate()
{    
    /*auto* input1 = getInput(0);
    auto* input2 = getInput(1);
    auto* output  = getOutput(0);
    if(input1->operation && input2->operation)
    {
        auto* src1 = input1->operation->getOutput(input1->index);
        auto* src2 = input2->operation->getOutput(input2->index);
        if (src1->type == src2->type)
            output->type = src1->type;
        else
            output->type = DataType_Error;
    }*/

}
//--------------------------------------------------------------
OperationData AddOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 + i2;
    return data;
}

size_t AddOperation::getPropertyCount() const
{
    return 2;
}
std::string AddOperation::getPropertyName(size_t i) const
{
    if (i==0)
        return "input1";
    if (i==1)
        return "input2";
    return "None";
}
OperationDataType AddOperation::getPropertyType(size_t i) const
{
    return DataType_Float;
}
void AddOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void AddOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}

//--------------------------------------------------------------
MultOperation::MultOperation()
{
    initialize({DataType_Float, DataType_Float},{DataType_Float});
}
//--------------------------------------------------------------
void MultOperation::validate()
{
    /*auto* input1 = getInput(0);
    auto* input2 = getInput(1);
    auto* output  = getOutput(0);
    if(input1->operation && input2->operation)
    {
        auto* src1 = input1->operation->getOutput(input1->index);
        auto* src2 = input2->operation->getOutput(input2->index);
        if (src1->type == src2->type)
            output->type = src1->type;
        else
            output->type = DataType_Error;
    }*/
}
//--------------------------------------------------------------
OperationData MultOperation::sample(size_t index, const Time& t)
{
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 * i2;
    return data;
}

size_t MultOperation::getPropertyCount() const
{
    return 2;
}
std::string MultOperation::getPropertyName(size_t i) const
{
    if (i==0)
        return "input1";
    if (i==1)
        return "input2";
    return "None";
}
OperationDataType MultOperation::getPropertyType(size_t i) const
{
    return DataType_Float;
}
void MultOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = input1;
    else if (i==1)
        value = input2;
}
void MultOperation::setProperty(size_t i, float value)
{
    if (i==0)
        input1 = value;
    else if (i==1)
        input2 = value;
}

//--------------------------------------------------------------
OutputOperation::OutputOperation()
{
    initialize({DataType_Float},{});
}
//--------------------------------------------------------------
void OutputOperation::validate()
{
    // auto* input = getInput(0);
}
//--------------------------------------------------------------
OperationData OutputOperation::sample(size_t index, const Time& t)
{
    return sampleInput(index, t);
}

size_t OutputOperation::getPropertyCount() const
{
    return 2;
}
std::string OutputOperation::getPropertyName(size_t i) const
{
    if (i == 0)
        return "range";
    else if (i==1)
        return "duration";
    return "None";
}
OperationDataType OutputOperation::getPropertyType(size_t i) const
{
    return DataType_Float;
}
void OutputOperation::getProperty(size_t i, float& value) const
{
    if (i==0)
        value = range;
    if (i==1)
        value = duration;
}
void OutputOperation::setProperty(size_t i, float value)
{
    if (i==0)
        range = value;
    if (i==1)
        duration = value;
}