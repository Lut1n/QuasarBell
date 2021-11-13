#include "App/BaseOperationNode.hpp"

#include "imgui.h"


//--------------------------------------------------------------
namespace qb
{
    template<typename Ty>
    Ty& getPropertyValueRef(BaseOperation::PropertyWrapper& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
    template<typename Ty>
    const Ty& getPropertyValueRef(const BaseOperation::PropertyWrapper& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
};

//--------------------------------------------------------------
BaseOperation::BaseOperation(size_t defaultTypeFlags)
    : _defaultTypeFlags(defaultTypeFlags)
{
}
//--------------------------------------------------------------
BaseOperationDataType BaseOperation::getInputType(size_t index) const
{
    if (index < inputs.size()) return inputs[index].type;
    return BaseOperationDataType::Undefined;
}
//--------------------------------------------------------------
BaseOperationDataType BaseOperation::getOuputType(size_t index) const
{
    if (index < outputs.size()) return outputs[index].type;
    return BaseOperationDataType::Undefined;
}
//--------------------------------------------------------------
void BaseOperation::setInput(size_t index, BaseOperation* src, size_t srcIndex)
{
    setConnection(src, srcIndex, this, index);
}
//--------------------------------------------------------------
void BaseOperation::setOuput(size_t index, BaseOperation* dst, size_t dstIndex)
{
    setConnection(this, index, dst, dstIndex);
}
//--------------------------------------------------------------
BaseOperationConnection* BaseOperation::getInput(size_t index)
{
    if (index < inputs.size())
        return &inputs[index];
    return nullptr;
}
//--------------------------------------------------------------
BaseOperationConnection* BaseOperation::getOutput(size_t index)
{
    if (index < outputs.size())
        return &outputs[index];
    return nullptr;
}
//--------------------------------------------------------------
void BaseOperation::startSamplingGraph(int d)
{ 
    for (auto input : inputs)
    {
        for(auto& ref : input.refs)
            if (ref.operation) ref.operation->startSamplingGraph(d+1);
    }
    startSampling(d);
}
//--------------------------------------------------------------
void BaseOperation::startSampling(int /*d*/)
{
    // to implement by children
}
//--------------------------------------------------------------
void BaseOperation::remOutput(size_t index, const BaseOperationConnection::Ref& toErase)
{
    if (index >= outputs.size()) return;

    auto& refs = outputs[index].refs;
    auto it = std::find_if(refs.begin(), refs.end(), [toErase](BaseOperationConnection::Ref& ref){
        return ref.operation == toErase.operation && ref.index == toErase.index;
    });
    refs.erase(it);
}

//--------------------------------------------------------------
void BaseOperation::setConnection(BaseOperation* src, size_t srcIdx, BaseOperation* dst, size_t dstIdx)
{
    remConnection(dst, dstIdx);
    
    if (src && dst && src != dst && srcIdx < src->outputs.size() && dstIdx < dst->inputs.size())
    {
        BaseOperationConnection::Ref ref;
        ref.operation = dst;
        ref.index = dstIdx;
        src->outputs[srcIdx].refs.push_back(ref);
        
        ref.operation = src;
        ref.index = srcIdx;
        dst->inputs[dstIdx].refs.push_back(ref);
        // dst->inputs[dstIdx].refs = {ref};
        dst->onInputConnectionChanged();
    }
}
//--------------------------------------------------------------
void BaseOperation::remConnection(BaseOperation* dst, size_t dstIdx)
{
    if (dst && dstIdx < dst->inputs.size() && dst->inputs[dstIdx].refs.size() > 0)
    {
        dst->onInputConnectionChanged();
        auto& inRefs = dst->inputs[dstIdx].refs[0];
        auto op = inRefs.operation;
        auto index = inRefs.index;
        if(op) op->remOutput(index, BaseOperationConnection::Ref{dst, dstIdx});
        dst->inputs[dstIdx].refs.clear();
    }
}
size_t BaseOperation::getPropertyCount() const
{
    return propertyWrappers.size();
}
//--------------------------------------------------------------
BaseOperationDataType BaseOperation::getPropertyType(size_t i) const
{
    if(i >= 0 && i < propertyWrappers.size())
        return std::get<1>(propertyWrappers[i]);
    return BaseOperationDataType::Error;
}
//--------------------------------------------------------------
std::string BaseOperation::getPropertyName(size_t i) const
{
    if(i >= 0 && i < propertyWrappers.size())
        return std::get<0>(propertyWrappers[i]);
    return "None";
}
//--------------------------------------------------------------
void BaseOperation::getProperty(size_t i, std::string& value) const
{
    value = qb::getPropertyValueRef<std::string>(propertyWrappers[i]);
}
//--------------------------------------------------------------
void BaseOperation::getProperty(size_t i, float& value) const
{
    value = qb::getPropertyValueRef<float>(propertyWrappers[i]);
}
//--------------------------------------------------------------
void BaseOperation::getProperty(size_t i, int& value) const
{
    value = qb::getPropertyValueRef<int>(propertyWrappers[i]);
}
//--------------------------------------------------------------
void BaseOperation::getPropertyMinMax(size_t i, float& minVal, float& maxVal) const
{
    auto minMaxVal = std::get<3>(propertyWrappers[i]);
    minVal = minMaxVal.f2[0];
    maxVal = minMaxVal.f2[1];
}
//--------------------------------------------------------------
void BaseOperation::getPropertyMinMax(size_t i, int& minVal, int& maxVal) const
{
    auto minMaxVal = std::get<3>(propertyWrappers[i]);
    minVal = minMaxVal.i2[0];
    maxVal = minMaxVal.i2[1];
}
//--------------------------------------------------------------
void BaseOperation::getProperty(size_t i, bool& value) const
{
    value = qb::getPropertyValueRef<bool>(propertyWrappers[i]);
}
//--------------------------------------------------------------
void BaseOperation::setProperty(size_t i, const std::string& value)
{
    qb::getPropertyValueRef<std::string>(propertyWrappers[i]) = value;
}
//--------------------------------------------------------------
void BaseOperation::setProperty(size_t i, float value)
{
    qb::getPropertyValueRef<float>(propertyWrappers[i]) = value;
}
//--------------------------------------------------------------
void BaseOperation::setProperty(size_t i, int value)
{
    qb::getPropertyValueRef<int>(propertyWrappers[i]) = value;
}
//--------------------------------------------------------------
void BaseOperation::setProperty(size_t i, bool value)
{
    qb::getPropertyValueRef<bool>(propertyWrappers[i]) = value;
}
//--------------------------------------------------------------
void BaseOperation::makeProperty(const std::string& name, BaseOperationDataType type, void* ptr)
{
    propertyWrappers.push_back({name, type, ptr, {0,0}});
}
//--------------------------------------------------------------
void BaseOperation::makeProperty(const std::string& name, float* ptr, float minVal, float maxVal)
{
    MinMaxVal mnmx; mnmx.f2[0] = minVal; mnmx.f2[1] = maxVal;
    propertyWrappers.push_back({name, BaseOperationDataType::Float, ptr, mnmx});
}
//--------------------------------------------------------------
void BaseOperation::makeProperty(const std::string& name, int* ptr, int minVal, int maxVal)
{
    MinMaxVal mnmx; mnmx.i2[0] = minVal; mnmx.i2[1] = maxVal;
    propertyWrappers.push_back({name, BaseOperationDataType::Int, ptr, mnmx});
}
//--------------------------------------------------------------
void BaseOperation::makeInput(const std::string& name, BaseOperationDataType type)
{
    makeInput(name, type, _defaultTypeFlags);
}
//--------------------------------------------------------------
void BaseOperation::makeOutput(const std::string& name, BaseOperationDataType type)
{
    makeOutput(name, type, _defaultTypeFlags);
}
//--------------------------------------------------------------
void BaseOperation::makeInput(const std::string& name, BaseOperationDataType type, size_t typeFlags)
{
    BaseOperationConnection input;
    input.name = name;
    input.type = type;
    input.pinTypeFlags = typeFlags;
    inputs.push_back(input);
}
//--------------------------------------------------------------
void BaseOperation::makeOutput(const std::string& name, BaseOperationDataType type, size_t typeFlags)
{
    BaseOperationConnection output;
    output.name = name;
    output.type = type;
    output.pinTypeFlags = typeFlags;
    outputs.push_back(output);
}
//--------------------------------------------------------------
void BaseOperation::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    for(size_t i=0; i<getPropertyCount(); ++i) uiProperty((int)i);
    ImGui::EndChild();
}
//--------------------------------------------------------------
void BaseOperation::uiProperty(int index)
{
    auto type = getPropertyType(index);
    auto name = getPropertyName(index);
    bool changed = false;

    if (type == BaseOperationDataType::Float)
    {
        float minVal = 0.0f, maxVal = 0.0f;
        getPropertyMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputFloat(name.c_str(), (float*)std::get<2>(propertyWrappers[index]));
        else
            changed = ImGui::SliderFloat(name.c_str(), (float*)std::get<2>(propertyWrappers[index]), minVal, maxVal);
    }
    else if (type == BaseOperationDataType::Int)
    {
        int minVal = 0, maxVal = 0;
        getPropertyMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputInt(name.c_str(), (int*)std::get<2>(propertyWrappers[index]));
        else
            changed = ImGui::SliderInt(name.c_str(), (int*)std::get<2>(propertyWrappers[index]), minVal, maxVal);
    }
    else if (type == BaseOperationDataType::Bool)
    {
        changed = ImGui::Checkbox(name.c_str(), (bool*)std::get<2>(propertyWrappers[index]));
    }

    if (changed) onPropertiesChanged();
}
//--------------------------------------------------------------
void BaseOperation::uiDebugIo()
{
    if(ImGui::CollapsingHeader("io"))
    {
        ImGui::Columns(2);
        ImGui::Text("in");
        ImGui::NextColumn();
        ImGui::Text("out");
        ImGui::NextColumn();
        ImGui::Separator();

        for(size_t i=0; i<getInputCount(); ++i)
        {
            auto* input = getInput(i);
            ImGui::Text(input->name.c_str());
            for(auto& ref : input->refs)
            {
                std::string sid = std::to_string(ref.index) + " ";
                ImGui::Text(sid.c_str());
            }
        }
        ImGui::NextColumn();
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto* output = getOutput(i);
            ImGui::Text(output->name.c_str());
            for(auto& ref : output->refs)
            {
                std::string sid = std::to_string(ref.index) + " ";
                ImGui::Text(sid.c_str());
            }
        }
        ImGui::NextColumn();

        ImGui::Columns(1);
    }
}
//--------------------------------------------------------------
void BaseOperation::saveCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
void BaseOperation::loadCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
bool BaseOperation::hasCustomData() const
{
    return _hasCustomData;
}
//--------------------------------------------------------------
bool BaseOperation::inputHasFlag(size_t index, size_t typeFlag) const
{
    if (index >= inputs.size()) return false;

    auto& co = inputs[index];
    if (co.refs.size() > 0 && co.refs[0].operation)
    {
        size_t inputTypeFlags = co.refs[0].operation->getOutputTypeFlags(co.refs[0].index);
        return qb::hasFlag(inputTypeFlags, typeFlag);
    }
    return false;
}
//--------------------------------------------------------------
BaseOperationNode::BaseOperationNode(const std::string& title, size_t nodeTypeId, BaseOperation* op)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
    unsigned basecolor = 0x001b1bFF;
    color = basecolor;
    colorOnOver = 0x505050FF;
    colorOnIdle = basecolor;
    rounded = true;
    _operation.reset(op);
    
    for(size_t i=0; i<_operation->getInputCount(); ++i)
    {
        auto input = _operation->getInput(i);
        addPin((int)i, input->name, false, input->pinTypeFlags);
    }
    for(size_t i=0; i<_operation->getOutputCount(); ++i)
    {
        auto output = _operation->getOutput(i);
        addPin((int)i, output->name, true, output->pinTypeFlags);
    }
}
//--------------------------------------------------------------
size_t BaseOperationNode::nodeTypeId() const
{
    return _nodetypeId;
}
//--------------------------------------------------------------
BaseOperation* BaseOperationNode::getOperation()
{
    return _operation.get();
}
//--------------------------------------------------------------
void BaseOperationNode::displayProperties()
{
    _operation->uiProperties();
}
//--------------------------------------------------------------
void BaseOperationNode::updatePreview()
{
}