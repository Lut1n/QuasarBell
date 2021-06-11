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

//--------------------------------------------------------------
ImagePreview::RenderFrame::RenderFrame(int resolution)
    : resolution(resolution)
{
    glResource = RenderInterface::createTarget(resolution,resolution,false);    
    RenderInterface::setTarget(glResource);
    RenderInterface::clear(0x000000FF);

    glProgram = RenderInterface::createCustomProgram();
}

//--------------------------------------------------------------
ImagePreview::RenderFrame::~RenderFrame()
{
    RenderInterface::deleteTarget(glResource);  
    RenderInterface::destroyCustomProgram(glProgram);
    glResource = 0;
    glProgram = 0;
}

//--------------------------------------------------------------
void ImagePreview::RenderFrame::reset()
{
    frames.clear();
    RenderInterface::resetCustomProgram(glProgram);
}

//--------------------------------------------------------------
void ImagePreview::RenderFrame::compute(qb::GlslFrame& frame)
{
    // if recompile
    size_t frameId = 0;
    frames.clear();
    frames.resize(frame.frames.size());
    for(auto& f2 : frame.frames)
    {
        frames[frameId].reset(new RenderFrame(resolution));
        frames[frameId]->compute(f2);
        frameId++;
    }
    
    opCode = frame.compile();
    RenderInterface::updateCustomProgram(glProgram, opCode, frame.hasUv);

    firstFrameUniformId = frame.inputs.size();

    RenderInterface::debugCheck("operation test B");

    updateUniforms(frame);
}

//--------------------------------------------------------------
void ImagePreview::RenderFrame::updateUniforms(qb::GlslFrame& frame)
{
    size_t frameId = 0;
    for(auto& f2 : frame.frames) frames[frameId++]->updateUniforms(f2);
    
    size_t inId = 0;
    for(auto& input : frame.inputs)
    {
        RenderInterface::debugCheck("Before update uniforms");
        RenderInterface::setInputCustomProgram(glProgram, inId++, input);
        RenderInterface::debugCheck("After update uniforms");
    }
    
    size_t keId = 0;
    for(auto& kernel : frame.kernels)
    {
        RenderInterface::debugCheck("Before update kernel");
        RenderInterface::setInputCustomProgram(glProgram, keId++, kernel);
        RenderInterface::debugCheck("After update kernel");
    }
}

//--------------------------------------------------------------
void ImagePreview::RenderFrame::render()
{
    for(auto& frame : frames) frame->render();

    size_t uniformId = 0;
    size_t textureUnit = 0;
    for (auto& frame : frames)
    {
        RenderInterface::debugCheck("Before update samplers");
        RenderInterface::setInputFrameCustomProgram(glProgram, frame->glResource, textureUnit++, uniformId++);
        RenderInterface::debugCheck("After update samplers");
    }

    RenderInterface::setTarget(glResource);
    RenderInterface::clear(0x000000FF);
    RenderInterface::applyCustomProgram(glProgram, vec2(0.0f,0.0f), vec2(resolution,resolution));
}

//--------------------------------------------------------------
ImageOperation::ImageOperation(qb::ImageOperationType opType)
    : _operationType(opType)
{
}
//--------------------------------------------------------------
void ImagePreview::dirty(bool recompile)
{
    hasChange = true;
    toRecompile = recompile;
}
//--------------------------------------------------------------
void ImagePreview::compute(ImageOperation* operation)
{
    if(!initialized)
    {
        renderFrame.reset(new RenderFrame(resolution));
        initialized = true;
    }
    if(!renderFrame) return;
    if(!hasChange && !toRecompile) return;

    operation->startSamplingGraph();
    ImageOperation::Time time;

    qb::GlslBuilderVisitor visitor;
    visitor.mainFrame.resolution = resolution;
    bool opValid = operation->sample(0, time, visitor);

    if(toRecompile)
    {
        RenderInterface::debugCheck("operation test A");
        if(opValid)
            renderFrame->compute(visitor.getCurrentFrame());
        else
            renderFrame->reset();
        toRecompile = false;
    }
    else if(opValid)
    {
        renderFrame->updateUniforms(visitor.getCurrentFrame());
    }

    RenderInterface::debugCheck("operation test C");

    renderFrame->render();
    hasChange = false;
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
        for(auto& ref : input.refs)
            if (ref.operation) ref.operation->startSamplingGraph();
    }
    startSampling();
}
//--------------------------------------------------------------
bool ImageOperation::sampleInput(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto* co = getInput(index);
    if (co->refs.size() > 0 && co->refs[0].operation)
    {
        return co->refs[0].operation->sample(co->refs[0].index, t, visitor);
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
    inputs.push_back(input);
}
//--------------------------------------------------------------
void ImageOperation::makeOutput(const std::string& name, ImageOperationDataType type)
{
    ImageOperationConnection output;
    output.name = name;
    output.type = type;
    outputs.push_back(output);
}
//--------------------------------------------------------------
void ImageOperation::setConnection(ImageOperation* src, size_t srcIdx, ImageOperation* dst, size_t dstIdx)
{
    remConnection(dst, dstIdx);
    
    if (src && dst && src != dst && srcIdx < src->outputs.size() && dstIdx < dst->inputs.size())
    {
        ImageOperationConnection::Ref ref;
        ref.operation = dst;
        ref.index = dstIdx;
        src->outputs[srcIdx].refs.push_back(ref);
        
        ref.operation = src;
        ref.index = srcIdx;
        dst->inputs[dstIdx].refs.push_back(ref);
        // dst->inputs[dstIdx].refs = {ref};
        dst->dirty(true);
    }
}
//--------------------------------------------------------------
void ImageOperation::remConnection(ImageOperation* dst, size_t dstIdx)
{
    if (dst && dstIdx < dst->inputs.size() && dst->inputs[dstIdx].refs.size() > 0)
    {
        dst->dirty(true);
        auto& inRefs = dst->inputs[dstIdx].refs[0];
        auto op = inRefs.operation;
        auto index = inRefs.index;
        if(op && index < op->outputs.size())
        {
            auto& outRefs = op->outputs[index].refs;
            ImageOperationConnection::Ref toErase{dst, dstIdx};
            auto it = std::find_if(outRefs.begin(), outRefs.end(), [toErase](ImageOperationConnection::Ref& ref){
                return ref.operation == toErase.operation && ref.index == toErase.index;
            });
            outRefs.erase(it);
        }
        dst->inputs[dstIdx].refs.clear();
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
//--------------------------------------------------------------
bool ImageOperation::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    // to implement by children
    // frame.setFunctions(getNodeType(), getOperationCode());
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
    ImGui::BeginChild("##properties");
    for(size_t i=0; i<getPropertyCount(); ++i) uiProperty((int)i);
    ImGui::EndChild();
}
//--------------------------------------------------------------
void ImageOperation::uiPreview()
{
    ImGui::Text("Preview:");
    float avail = ImGui::GetContentRegionAvail().x * 0.5f;
    if(preview.renderFrame)
    {
        ImGui::Image(RenderInterface::getTargetResource(preview.renderFrame->glResource), ImVec2(avail, avail), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            ImGui::OpenPopup("Bigger preview");
        
        if(ImGui::BeginPopup("Bigger preview"))
        {
            ImGui::Image(RenderInterface::getTargetResource(preview.renderFrame->glResource), ImVec2(512, 512), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
            ImGui::EndPopup();
        }
    }

    if(ImGui::CollapsingHeader("glsl"))
    {
        if(preview.renderFrame)
            ImGui::Text(preview.renderFrame->opCode.c_str());
    }
    ImGui::Separator();
    uiDebugIo();
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

    if (changed) dirty(false);
}

void ImageOperation::uiDebugIo()
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

void ImageOperation::dirty(bool recompile)
{
    preview.dirty(recompile);
    for(auto& output : outputs)
    {
        for(auto& ref : output.refs)
            ref.operation->dirty(recompile);
    }
}

qb::ImageOperationType ImageOperation::getNodeType() const
{
    return _operationType;
}