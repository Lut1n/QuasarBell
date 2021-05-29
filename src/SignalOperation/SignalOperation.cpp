#include "SignalOperation/SignalOperation.hpp"

#include <algorithm>

#include "imgui.h"

//--------------------------------------------------------------
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
void SignalPreview::dirty()
{
    hasChange = true;
}
//--------------------------------------------------------------
void SignalPreview::compute(SignalOperation* operation)
{
    const int sample_count = (int)data.size();
    if(hasChange)
    {
        operation->startSamplingGraph();
        SignalOperation::Time time;
        time.duration = 1.0;
        time.elapsed = 1.0f/sample_count;
        time.dstOp = operation;
        float minVal = 100000.0; float maxVal = -100000.0;
        for(size_t i=0; i<sample_count; ++i)
        {
            time.t = (float)i/sample_count;
            time.sec = (float)i/sample_count;

            data[i] = operation->sample(0, time).fvec[0];
            if (data[i] < minVal) minVal = data[i];
            if (data[i] > maxVal) maxVal = data[i];
        }
        this->maxVal = abs(maxVal) > abs(minVal) ? abs(maxVal) : abs(minVal);
        hasChange = false;
    }
}
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
void SignalOperation::startSamplingGraph()
{ 
    for (auto input : inputs)
    {
        if (input.operation)
            input.operation->startSamplingGraph();
    }
    startSampling();
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
void SignalOperation::makeProperty(const PropDesc& property)
{
    propDescs.push_back(property);
}
//--------------------------------------------------------------
void SignalOperation::makeInput(const std::string& name, OperationDataType type)
{
    SignalOperationConnection input;
    input.name = name;
    input.type = type;
    input.operation = nullptr;
    input.index = 0;
    inputs.push_back(input);
}
//--------------------------------------------------------------
void SignalOperation::makeOutput(const std::string& name, OperationDataType type)
{
    SignalOperationConnection output;
    output.name = name;
    output.type = type;
    output.operation = nullptr;
    output.index = 0;
    outputs.push_back(output);
}
//--------------------------------------------------------------
void SignalOperation::setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx)
{
    remConnection(dst, dstIdx);
    
    if (src && dst && src != dst && srcIdx < src->outputs.size() && dstIdx < dst->inputs.size())
    {
        src->outputs[srcIdx].operation = dst;
        src->outputs[srcIdx].index = dstIdx;
        
        dst->inputs[dstIdx].operation = src;
        dst->inputs[dstIdx].index = srcIdx;
        dst->preview.dirty();
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
        dst->preview.dirty();
    }
}
//--------------------------------------------------------------
std::string SignalOperation::name() const
{
    return "None";
}
//--------------------------------------------------------------
void SignalOperation::startSampling()
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
//--------------------------------------------------------------
size_t SignalOperation::getPropertyCount() const
{
    return propDescs.size();
}
//--------------------------------------------------------------
OperationDataType SignalOperation::getPropertyType(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<1>(propDescs[i]);
    return DataType_Error;
}
//--------------------------------------------------------------
std::string SignalOperation::getPropertyName(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<0>(propDescs[i]);
    return "None";
}
//--------------------------------------------------------------
void SignalOperation::getProperty(size_t i, std::string& value) const
{
    value = qb::getPropertyValueRef<std::string>(propDescs[i]);
}
//--------------------------------------------------------------
void SignalOperation::getProperty(size_t i, float& value) const
{
    value = qb::getPropertyValueRef<float>(propDescs[i]);
}
//--------------------------------------------------------------
void SignalOperation::getProperty(size_t i, int& value) const
{
    value = qb::getPropertyValueRef<int>(propDescs[i]);
}
//--------------------------------------------------------------
void SignalOperation::getProperty(size_t i, bool& value) const
{
    value = qb::getPropertyValueRef<bool>(propDescs[i]);
}
//--------------------------------------------------------------
void SignalOperation::setProperty(size_t i, const std::string& value)
{
    qb::getPropertyValueRef<std::string>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void SignalOperation::setProperty(size_t i, float value)
{
    qb::getPropertyValueRef<float>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void SignalOperation::setProperty(size_t i, int value)
{
    qb::getPropertyValueRef<int>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void SignalOperation::setProperty(size_t i, bool value)
{
    qb::getPropertyValueRef<bool>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void SignalOperation::saveCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
void SignalOperation::loadCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
bool SignalOperation::hasCustomData() const
{
    return _hasCustomData;
}
//--------------------------------------------------------------
void SignalOperation::uiProperties()
{
    ImGui::Text("Preview:");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    
    ImGui::Separator();
    for(size_t i=0; i<getPropertyCount(); ++i) uiProperty((int)i);
}
//--------------------------------------------------------------
void SignalOperation::uiProperty(int index)
{
    auto type = getPropertyType(index);
    auto name = getPropertyName(index);
    bool changed = false;
    if (type == DataType_Float)
        changed = ImGui::InputFloat(name.c_str(), (float*)std::get<2>(propDescs[index]));
    else if (type == DataType_Int)
        changed = ImGui::InputInt(name.c_str(), (int*)std::get<2>(propDescs[index]));
    else if (type == DataType_Bool)
        changed = ImGui::Checkbox(name.c_str(), (bool*)std::get<2>(propDescs[index]));

    if (changed) preview.dirty();
}