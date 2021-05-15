#include "ImageOperation/ImageOperation.hpp"

#include <algorithm>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
namespace qb
{
    template<typename Ty>
    Ty& getImagePropertyValueRef(ImageOperation::PropDesc& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
    template<typename Ty>
    const Ty& getImagePropertyValueRef(const ImageOperation::PropDesc& property)
    {
        return *reinterpret_cast<Ty*>(std::get<2>(property));
    }
};
ImagePreview::~ImagePreview()
{
    RenderInterface::deleteTarget(glResource);  
    RenderInterface::destroyCustomProgram(glProgram);
    glResource = 0;
    glProgram = 0;
}
//--------------------------------------------------------------
ImageOperation::ImageOperation(qb::ImageOperationType opType)
    : _operationType(opType)
{
}
//--------------------------------------------------------------
void ImagePreview::dirty()
{
    hasChange = true;
}
//--------------------------------------------------------------
void ImagePreview::compute(ImageOperation* operation)
{
    if(!initialized)
    {
        glResource = RenderInterface::createTarget(res,res,false);    
        RenderInterface::setTarget(glResource);
        RenderInterface::clear(0x0000FFFF);

        glProgram = RenderInterface::createCustomProgram();

        initialized = true;
    }
    if(hasChange)
    {
        operation->startSamplingGraph();
        ImageOperation::Time time;
        time.size = {(float)res,(float)res};
        time.coord = {0.0f,0.0f};
        time.uv = {0.0f, 0.0f};
        time.dstOp = operation;

        opCode.clear();
        ImageOperationData data;
        if(operation->sample(0, time, data))
        {
            opCode += std::string("#version 330\n");

            for(size_t i=0; i<data.collectedUniforms.size(); ++i)
            {
                opCode += std::string("uniform vec4 ") + std::string("u") + std::to_string(i) + std::string(" = ") + std::string("vec4(") + 
                std::to_string(data.collectedUniforms[i].x) + std::string(",") + 
                std::to_string(data.collectedUniforms[i].y) + std::string(",") + 
                std::to_string(data.collectedUniforms[i].z) + std::string(",") + 
                std::to_string(data.collectedUniforms[i].w) + std::string(");\n");
            }

            if (data.useUV) opCode += "in vec2 vUV;\n";
            opCode += "out vec4 fragColor;\n";

            for(auto funcIt : data.collectedFunctions) opCode += funcIt.second;

            opCode += std::string("void main(){\n");

            for(size_t i=0; i<data.collectedOperations.size(); ++i)
                opCode += std::string("    ") + data.collectedOperations[i];

            opCode += std::string("    ") + std::string("fragColor = ") + std::string("v") + std::to_string(data.collectedOperations.size()-1) + std::string(";\n");
            opCode += std::string("};\n");

            RenderInterface::updateCustomProgram(glProgram, opCode);

            Rect surface = Rect::fromPosAndSize(vec2(0.0f,0.0f), vec2(res,res));
            RenderInterface::setTarget(glResource);
            RenderInterface::clear(0x0000FFFF);
            RenderInterface::applyCustomProgram(glProgram, surface.p0, surface.p1);
        }
        
        hasChange = false;
    }
}
//--------------------------------------------------------------
ImageOperationDataType ImageOperation::getInputType(size_t index) const
{
    if (index < inputs.size()) return inputs[index].type;
    return ImageDataType_Undefined;
}
//--------------------------------------------------------------
ImageOperationDataType ImageOperation::getOuputType(size_t index) const
{
    if (index < outputs.size()) return outputs[index].type;
    return ImageDataType_Undefined;
}
//--------------------------------------------------------------
void ImageOperation::setInput(size_t index, ImageOperation* src, size_t srcIndex)
{
    setConnection(src, srcIndex, this, index);
}
//--------------------------------------------------------------
void ImageOperation::setOuput(size_t index, ImageOperation* dst, size_t dstIndex)
{
    setConnection(this, index, dst, dstIndex);
}
//--------------------------------------------------------------
ImageOperation* ImageOperation::getInputOperation(size_t index)
{
    return inputs[index].operation;
}
//--------------------------------------------------------------
ImageOperationConnection* ImageOperation::getInput(size_t index)
{
    if (index < inputs.size())
        return &inputs[index];
    return nullptr;
}
//--------------------------------------------------------------
ImageOperationConnection* ImageOperation::getOutput(size_t index)
{
    if (index < outputs.size())
        return &outputs[index];
    return nullptr;
}
//--------------------------------------------------------------
void ImageOperation::startSamplingGraph()
{ 
    for (auto input : inputs)
    {
        if (input.operation)
            input.operation->startSamplingGraph();
    }
    startSampling();
}
//--------------------------------------------------------------
bool ImageOperation::sampleInput(size_t index, const Time& t, ImageOperationData& data)
{
    auto* co = getInput(index);
    if (co->operation)
    {
        return co->operation->sample(inputs[index].index, t, data);
    }
    else
        return false;
}
//--------------------------------------------------------------
void ImageOperation::makeProperty(const std::string& name, ImageOperationDataType type, void* ptr)
{
    propDescs.push_back({name, type, ptr, {0,0}});
}
//--------------------------------------------------------------
void ImageOperation::makeProperty(const std::string& name, float* ptr, float minVal, float maxVal)
{
    MinMaxVal mnmx; mnmx.f2[0] = minVal; mnmx.f2[1] = maxVal;
    propDescs.push_back({name, ImageDataType_Float, ptr, mnmx});
}
//--------------------------------------------------------------
void ImageOperation::makeProperty(const std::string& name, int* ptr, int minVal, int maxVal)
{
    MinMaxVal mnmx; mnmx.i2[0] = minVal; mnmx.i2[1] = maxVal;
    propDescs.push_back({name, ImageDataType_Int, ptr, mnmx});
}
//--------------------------------------------------------------
void ImageOperation::makeInput(const std::string& name, ImageOperationDataType type)
{
    ImageOperationConnection input;
    input.name = name;
    input.type = type;
    input.operation = nullptr;
    input.index = 0;
    inputs.push_back(input);
}
//--------------------------------------------------------------
void ImageOperation::makeOutput(const std::string& name, ImageOperationDataType type)
{
    ImageOperationConnection output;
    output.name = name;
    output.type = type;
    output.operation = nullptr;
    output.index = 0;
    outputs.push_back(output);
}
//--------------------------------------------------------------
void ImageOperation::setConnection(ImageOperation* src, size_t srcIdx, ImageOperation* dst, size_t dstIdx)
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
void ImageOperation::remConnection(ImageOperation* dst, size_t dstIdx)
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
std::string ImageOperation::name() const
{
    return "None";
}
//--------------------------------------------------------------
void ImageOperation::startSampling()
{
    // to implement by children
}
void ImageOperation::addOperationCode(ImageOperationData& data)
{
    if(data.collectedFunctions.find(_operationType) == data.collectedFunctions.end())
        data.collectedFunctions[_operationType] = getOperationCode();
}
//--------------------------------------------------------------
bool ImageOperation::sample(size_t index, const Time& t, ImageOperationData& data)
{
    // to implement by children
    // addOperationCode(data);
    return false;
}
std::string ImageOperation::getOperationCode() const
{
    return std::string();
}
//--------------------------------------------------------------
size_t ImageOperation::getInputCount() const
{
    return inputs.size();
}
//--------------------------------------------------------------
size_t ImageOperation::getOutputCount() const
{
    return outputs.size();
}
//--------------------------------------------------------------
size_t ImageOperation::getPropertyCount() const
{
    return propDescs.size();
}
//--------------------------------------------------------------
ImageOperationDataType ImageOperation::getPropertyType(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<1>(propDescs[i]);
    return ImageDataType_Error;
}
//--------------------------------------------------------------
std::string ImageOperation::getPropertyName(size_t i) const
{
    if(i >= 0 && i < propDescs.size())
        return std::get<0>(propDescs[i]);
    return "None";
}
//--------------------------------------------------------------
void ImageOperation::getProperty(size_t i, std::string& value) const
{
    value = qb::getImagePropertyValueRef<std::string>(propDescs[i]);
}
//--------------------------------------------------------------
void ImageOperation::getProperty(size_t i, float& value) const
{
    value = qb::getImagePropertyValueRef<float>(propDescs[i]);
}
//--------------------------------------------------------------
void ImageOperation::getProperty(size_t i, int& value) const
{
    value = qb::getImagePropertyValueRef<int>(propDescs[i]);
}
//--------------------------------------------------------------
void ImageOperation::getPropertyMinMax(size_t i, float& minVal, float& maxVal) const
{
    auto minMaxVal = std::get<3>(propDescs[i]);
    minVal = minMaxVal.f2[0];
    maxVal = minMaxVal.f2[1];
}
//--------------------------------------------------------------
void ImageOperation::getPropertyMinMax(size_t i, int& minVal, int& maxVal) const
{
    auto minMaxVal = std::get<3>(propDescs[i]);
    minVal = minMaxVal.i2[0];
    maxVal = minMaxVal.i2[1];
}
//--------------------------------------------------------------
void ImageOperation::getProperty(size_t i, bool& value) const
{
    value = qb::getImagePropertyValueRef<bool>(propDescs[i]);
}
//--------------------------------------------------------------
void ImageOperation::setProperty(size_t i, const std::string& value)
{
    qb::getImagePropertyValueRef<std::string>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void ImageOperation::setProperty(size_t i, float value)
{
    qb::getImagePropertyValueRef<float>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void ImageOperation::setProperty(size_t i, int value)
{
    qb::getImagePropertyValueRef<int>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void ImageOperation::setProperty(size_t i, bool value)
{
    qb::getImagePropertyValueRef<bool>(propDescs[i]) = value;
}
//--------------------------------------------------------------
void ImageOperation::saveCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
void ImageOperation::loadCustomData(JsonValue& json)
{
}
//--------------------------------------------------------------
bool ImageOperation::hasCustomData() const
{
    return _hasCustomData;
}
//--------------------------------------------------------------
void ImageOperation::uiProperties()
{
    uiPreview();
    for(size_t i=0; i<getPropertyCount(); ++i) uiProperty((int)i);
}
//--------------------------------------------------------------
void ImageOperation::uiPreview()
{
    ImGui::Text("Preview:");
    float avail = ImGui::GetContentRegionAvail().x * 0.5f;
    ImGui::Image(RenderInterface::getTargetResource(preview.glResource), ImVec2(avail, avail), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));

    if(ImGui::CollapsingHeader("glsl"))
    {
        ImGui::Text(preview.opCode.c_str());
    }
    ImGui::Separator();
}
//--------------------------------------------------------------
void ImageOperation::uiProperty(int index)
{
    auto type = getPropertyType(index);
    auto name = getPropertyName(index);
    bool changed = false;

    if (type == ImageDataType_Float)
    {
        float minVal = 0.0f, maxVal = 0.0f;
        getPropertyMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputFloat(name.c_str(), (float*)std::get<2>(propDescs[index]));
        else
            changed = ImGui::SliderFloat(name.c_str(), (float*)std::get<2>(propDescs[index]), minVal, maxVal);
    }
    else if (type == ImageDataType_Int)
    {
        int minVal = 0, maxVal = 0;
        getPropertyMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputInt(name.c_str(), (int*)std::get<2>(propDescs[index]));
        else
            changed = ImGui::SliderInt(name.c_str(), (int*)std::get<2>(propDescs[index]), minVal, maxVal);
    }
    else if (type == ImageDataType_Bool)
    {
        changed = ImGui::Checkbox(name.c_str(), (bool*)std::get<2>(propDescs[index]));
    }

    if (changed) dirty();
}

void ImageOperation::dirty()
{
    preview.dirty();
    for(auto& output : outputs)
    {
        if(output.operation) output.operation->dirty();
    }
}