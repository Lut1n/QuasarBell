#include "ImageOperation/ImageOperation.hpp"

#include <algorithm>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
ImagePreview::RenderFrame::RenderFrame(int resolution)
    : resolution(resolution)
{
    glResource = RenderInterface::createTarget(resolution,resolution,false);    
    RenderInterface::setTarget((unsigned)glResource);
    RenderInterface::clear(0x000000FF);

    glProgram = RenderInterface::createCustomProgram();
}

//--------------------------------------------------------------
ImagePreview::RenderFrame::~RenderFrame()
{
    RenderInterface::deleteTarget((unsigned)glResource);  
    RenderInterface::destroyCustomProgram((unsigned)glProgram);
    glResource = 0;
    glProgram = 0;
}

//--------------------------------------------------------------
void ImagePreview::RenderFrame::reset()
{
    frames.clear();
    RenderInterface::resetCustomProgram((unsigned)glProgram);
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
    RenderInterface::updateCustomProgram((unsigned)glProgram, opCode, frame.hasUv);

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
        RenderInterface::setInputCustomProgram((unsigned)glProgram, inId++, input);
        RenderInterface::debugCheck("After update uniforms");
    }
    
    size_t keId = 0;
    for(auto& kernel : frame.kernels)
    {
        RenderInterface::debugCheck("Before update kernel");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, keId++, kernel);
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
        RenderInterface::setInputFrameCustomProgram((unsigned)glProgram, frame->glResource, textureUnit++, uniformId++);
        RenderInterface::debugCheck("After update samplers");
    }

    RenderInterface::setTarget((unsigned)glResource);
    RenderInterface::clear(0x000000FF);
    RenderInterface::applyCustomProgram((unsigned)glProgram, vec2(0.0f,0.0f), vec2((float)resolution,(float)resolution));
}

//--------------------------------------------------------------
ImageOperation::ImageOperation(qb::ImageOperationType opType)
    : BaseOperation(UiPin::TYPE_FLOAT2)
    , _operationType(opType)
{
}
//--------------------------------------------------------------
void ImagePreview::dirty(bool recompile)
{
    hasChange = true;
    if(recompile) toRecompile = true;
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

    qb::GlslBuilderVisitor visitor;
    visitor.mainFrame.resolution = resolution;
    bool opValid = operation->sample(0, visitor);

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
bool ImageOperation::sampleInput(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto* co = getInput(index);
    if (co->refs.size() > 0 && co->refs[0].operation)
    {
        return co->refs[0].operation->sample(co->refs[0].index, visitor);
    }
    else
        return false;
}
//--------------------------------------------------------------
std::string ImageOperation::pushOpOrInput(size_t index, qb::GlslBuilderVisitor& visitor, const vec4& uniform)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    if (sampleInput(index, visitor))
        return qb::va(context.popVa());
    else
        return qb::in(frame.pushInput(uniform));
}
//--------------------------------------------------------------
std::string ImageOperation::name() const
{
    return "None";
}
//--------------------------------------------------------------
bool ImageOperation::sample(size_t index, BaseOperationVisitor& visitor)
{
    return sample(index, dynamic_cast<qb::GlslBuilderVisitor&>(visitor));
}
//--------------------------------------------------------------
bool ImageOperation::sample(size_t index, qb::GlslBuilderVisitor& visitor)
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
void ImageOperation::uiPreview()
{
    ImGui::Text("Preview:");
    float avail = ImGui::GetContentRegionAvail().x * 0.5f;
    if(preview.renderFrame)
    {
        ImVec2 cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, cursor.y));
        ImGui::Image(RenderInterface::getTargetResource((unsigned)preview.renderFrame->glResource), ImVec2(avail, avail), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            ImGui::OpenPopup("Bigger preview");
        
        if(ImGui::BeginPopup("Bigger preview"))
        {
            ImGui::Image(RenderInterface::getTargetResource((unsigned)preview.renderFrame->glResource), ImVec2(512, 512), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
            ImGui::EndPopup();
        }
    }

    if(ImGui::CollapsingHeader("glsl"))
    {
        if(preview.renderFrame)
            ImGui::Text(preview.renderFrame->opCode.c_str());
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            ImGui::OpenPopup("Bigger glsl");
        
        if(ImGui::BeginPopup("Bigger glsl"))
        {
            ImGui::Text("Copyable glsl code");
            std::string ccp = preview.renderFrame->opCode;
            ImGui::InputTextMultiline("##glsl-code", ccp.data(), ccp.size(), ImVec2(512, 512), ImGuiInputTextFlags_ReadOnly);
            ImGui::EndPopup();
        }
    }
    ImGui::Separator();
    uiDebugIo();
    ImGui::Separator();
}
//--------------------------------------------------------------
void ImageOperation::onInputConnectionChanged()
{
    dirty(true);
}
//--------------------------------------------------------------
void ImageOperation::onPropertiesChanged()
{
    dirty(false);
}
void ImageOperation::dirty(bool recompile)
{
    preview.dirty(recompile);
    for(auto& output : outputs)
    {
        for(auto& ref : output.refs)
        {
            dynamic_cast<ImageOperation*>(ref.operation)->dirty(recompile);
        }
    }
}

qb::ImageOperationType ImageOperation::getNodeType() const
{
    return _operationType;
}