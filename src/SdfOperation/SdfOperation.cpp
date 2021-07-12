#include "SdfOperation/SdfOperation.hpp"

#include "ImageOperation/GlslBuilder.hpp"
#include "App/AppInterface.hpp"

#include <algorithm>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
RMPreview::RenderFrame::RenderFrame(int resolution)
    : resolution(resolution)
{
    glResource = RenderInterface::createTarget(resolution,resolution,false);    
    RenderInterface::setTarget((unsigned)glResource);
    RenderInterface::clear(0x000000FF);

    glProgram = RenderInterface::createCustomProgram();
}

//--------------------------------------------------------------
RMPreview::RenderFrame::~RenderFrame()
{
    RenderInterface::deleteTarget((unsigned)glResource);  
    RenderInterface::destroyCustomProgram((unsigned)glProgram);
    glResource = 0;
    glProgram = 0;
}

//--------------------------------------------------------------
void RMPreview::RenderFrame::reset()
{
    frames.clear();
    RenderInterface::resetCustomProgram((unsigned)glProgram);
}

//--------------------------------------------------------------
void RMPreview::RenderFrame::compute(qb::RMFrame& frame)
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
void RMPreview::RenderFrame::updateUniforms(qb::RMFrame& frame)
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
void RMPreview::RenderFrame::render()
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
SdfOperation::SdfOperation(qb::SdfOperationType opType)
    : BaseOperation(UiPin::TYPE_FLOAT3)
    , _operationType(opType)
{
}
//--------------------------------------------------------------
void RMPreview::dirty(bool recompile)
{
    hasChange = true;
    if(recompile) toRecompile = true;
}
//--------------------------------------------------------------
void RMPreview::compute(SdfOperation* operation)
{
    if(!initialized)
    {
        renderFrame.reset(new RenderFrame(resolution));
        initialized = true;
    }
    if(!renderFrame) return;
    if(!hasChange && !toRecompile) return;

    operation->startSamplingGraph();

    qb::RMBuilderVisitor visitor;
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
bool SdfOperation::sampleInput(size_t index, qb::RMBuilderVisitor& visitor)
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
std::string SdfOperation::pushOpOrInput(size_t index, qb::RMBuilderVisitor& visitor, const vec4& uniform)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    if (sampleInput(index, visitor))
        return qb::va(context.popVa());
    else
        return qb::in(frame.pushInput(uniform));
}
//--------------------------------------------------------------
std::string SdfOperation::name() const
{
    return "None";
}
//--------------------------------------------------------------
bool SdfOperation::sample(size_t index, BaseOperationVisitor& visitor)
{
    return sample(index, dynamic_cast<qb::RMBuilderVisitor&>(visitor));
}
//--------------------------------------------------------------
bool SdfOperation::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    // to implement by children
    // frame.setFunctions(getNodeType(), getOperationCode());
    return false;
}
std::string SdfOperation::getOperationCode() const
{
    return std::string();
}
//--------------------------------------------------------------
void SdfOperation::uiPreview()
{
    ImGui::Text("Preview:");
    float avail = ImGui::GetContentRegionAvail().x * 0.5f;
    if(preview.renderFrame)
    {
        ImVec2 cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, cursor.y));
        ImGui::Image(RenderInterface::getTargetResource((unsigned)preview.renderFrame->glResource), ImVec2(avail, avail), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        {
            AppInterface::get().openBigPreview(this);
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
void SdfOperation::onInputConnectionChanged()
{
    dirty(true);
}
//--------------------------------------------------------------
void SdfOperation::onPropertiesChanged()
{
    dirty(false);
}
void SdfOperation::dirty(bool recompile)
{
    preview.dirty(recompile);
    for(auto& output : outputs)
    {
        for(auto& ref : output.refs)
        {
            dynamic_cast<SdfOperation*>(ref.operation)->dirty(recompile);
        }
    }
}

qb::SdfOperationType SdfOperation::getNodeType() const
{
    return _operationType;
}