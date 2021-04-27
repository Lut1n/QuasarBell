#include "signal/operations/SignalOperation.hpp"

#include <algorithm>


namespace qb
{
    template<typename Ty>
    Ty& getPropertyValueRef(SignalOperation::PropDesc& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
    template<typename Ty>
    const Ty& getPropertyValueRef(const SignalOperation::PropDesc& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
};

//--------------------------------------------------------------
OperationDataType SignalOperation::getInputType(size_t index) const
{
    if (index < inputs.size()) return inputs[index].type;
    return DataType_Undefined;
}

//--------------------------------------------------------------
OperationDataType SignalOperation::getOuputType(size_t index) const
{
    if (index < outputs.size()) return outputs[index].type;
    return DataType_Undefined;
}

//--------------------------------------------------------------
void SignalOperation::setInput(size_t index, SignalOperation* src, size_t srcIndex)
{
    setConnection(src, srcIndex, this, index);
}

//--------------------------------------------------------------
void SignalOperation::setOuput(size_t index, SignalOperation* dst, size_t dstIndex)
{
    setConnection(this, index, dst, dstIndex);
}

//--------------------------------------------------------------
SignalOperation* SignalOperation::getInputOperation(size_t index)
{
    return inputs[index].operation;
}

//--------------------------------------------------------------
SignalOperationConnection* SignalOperation::getInput(size_t index)
{
    if (index < inputs.size())
        return &inputs[index];
    return nullptr;
}

//--------------------------------------------------------------
SignalOperationConnection* SignalOperation::getOutput(size_t index)
{
    if (index < outputs.size())
        return &outputs[index];
    return nullptr;
}

//--------------------------------------------------------------
void SignalOperation::validateGraph()
{ 
    for (auto input : inputs)
    {
        if (input.operation)
            input.operation->validateGraph();
    }
    validate();
}

//--------------------------------------------------------------
OperationData SignalOperation::sampleInput(size_t index, const Time& t)
{
    auto* co = getInput(index);
    if (co->operation)
    {
        return co->operation->sample(inputs[index].index, t);
    }
    else
        return OperationData{DataType_Error};
}

//--------------------------------------------------------------
void SignalOperation::initialize(const std::vector<OperationDataType>& input, const std::vector<OperationDataType>& output)
{
    inputs.resize(input.size());
    for (int i=0; i<inputs.size(); ++i)
    {
        inputs[i].type = input[i];
        inputs[i].index = 0;
        inputs[i].operation = nullptr;
    }
    outputs.resize(output.size());
    for (int i=0; i<outputs.size(); ++i)
    {
        outputs[i].type = output[i];
        outputs[i].index = 0;
        outputs[i].operation = nullptr;
    }
}

//--------------------------------------------------------------
void SignalOperation::makeProperty(const PropDesc& property)
{
    propDescs.push_back(property);
}

//--------------------------------------------------------------
void SignalOperation::setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx)
{
    remConnection(dst, dstIdx);
    
    if (src && dst && src != dst && srcIdx < src->outputs.size() && dstIdx < dst->inputs.size())
    {
        // std::cout << "create connection " << std::hex << src << "#" << srcIdx << " - " << std::hex << dst << "#" << dstIdx << std::endl;
        src->outputs[srcIdx].operation = dst;
        src->outputs[srcIdx].index = dstIdx;
        
        dst->inputs[dstIdx].operation = src;
        dst->inputs[dstIdx].index = srcIdx;
    }
}

//--------------------------------------------------------------
void SignalOperation::remConnection(SignalOperation* dst, size_t dstIdx)
{
    if (dst && dstIdx < dst->inputs.size())
    {
        auto op = dst->inputs[dstIdx].operation;
        auto index = dst->inputs[dstIdx].index;
        if(op && index < op->outputs.size())
        {
            op->outputs[index].operation = nullptr;
            op->outputs[index].index = 0;
        }
        dst->inputs[dstIdx].operation = nullptr;
        dst->inputs[dstIdx].index = 0;
    }
}

//--------------------------------------------------------------
std::string SignalOperation::name() const
{
    return "None";
}

//--------------------------------------------------------------
void SignalOperation::validate()
{
    // to implement by children
}

//--------------------------------------------------------------
OperationData SignalOperation::sample(size_t index, const Time& t)
{
    // to implement by children
    return OperationData{DataType_Error};
}

//--------------------------------------------------------------
size_t SignalOperation::getInputCount() const
{
    return inputs.size();
}

//--------------------------------------------------------------
size_t SignalOperation::getOutputCount() const
{
    return outputs.size();
}

size_t SignalOperation::getPropertyCount() const
{
    return propDescs.size();
}
OperationDataType SignalOperation::getPropertyType(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<1>(propDescs[i]);
    return DataType_Error;
}
std::string SignalOperation::getPropertyName(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<0>(propDescs[i]);
    return "None";
}

void SignalOperation::getProperty(size_t i, std::string& value) const
{
    value = qb::getPropertyValueRef<std::string>(propDescs[i]);
}
void SignalOperation::getProperty(size_t i, float& value) const
{
    value = qb::getPropertyValueRef<float>(propDescs[i]);
}
void SignalOperation::getProperty(size_t i, int& value) const
{
    value = qb::getPropertyValueRef<int>(propDescs[i]);
}
void SignalOperation::getProperty(size_t i, bool& value) const
{
    value = qb::getPropertyValueRef<bool>(propDescs[i]);
}
void SignalOperation::setProperty(size_t i, const std::string& value)
{
    qb::getPropertyValueRef<std::string>(propDescs[i]) = value;
}
void SignalOperation::setProperty(size_t i, float value)
{
    qb::getPropertyValueRef<float>(propDescs[i]) = value;
}
void SignalOperation::setProperty(size_t i, int value)
{
    qb::getPropertyValueRef<int>(propDescs[i]) = value;
}
void SignalOperation::setProperty(size_t i, bool value)
{
    qb::getPropertyValueRef<bool>(propDescs[i]) = value;
}
void SignalOperation::saveCustomData(JsonValue& json)
{
}
void SignalOperation::loadCustomData(JsonValue& json)
{
}

bool SignalOperation::hasCustomData() const
{
    return _hasCustomData;
}