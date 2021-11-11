#include "SignalOperation/SignalOperation.hpp"

#include <algorithm>

#include "imgui.h"

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
        qb::PcmBuilderVisitor visitor;
        visitor.time.duration = 1.0;
        visitor.time.elapsed = 1.0f/sample_count;
        float minVal = 100000.0; float maxVal = -100000.0;
        for(size_t i=0; i<sample_count; ++i)
        {
            visitor.time.t = (float)i/sample_count;
            visitor.time.sec = (float)i/sample_count;

            operation->sample(0, visitor);
            data[i] = visitor.data.fvec[0];
            if (data[i] < minVal) minVal = data[i];
            if (data[i] > maxVal) maxVal = data[i];
        }
        this->maxVal = abs(maxVal) > abs(minVal) ? abs(maxVal) : abs(minVal);
        hasChange = false;
    }
}
//--------------------------------------------------------------
SignalOperation::SignalOperation()
    : BaseOperation(UiPin::TYPE_FLOAT1)
{
}
//--------------------------------------------------------------
bool SignalOperation::sampleInput(size_t index, qb::PcmBuilderVisitor& visitor)
{
    auto* co = getInput(index);
    if (!co->refs.empty())
    {
        auto& ref = co->refs[0];
        return ref.operation->sample(ref.index, visitor);
    }
    else
        return false;
}
//--------------------------------------------------------------
float SignalOperation::inputOrProperty(size_t index, qb::PcmBuilderVisitor& visitor, float property)
{
    qb::PcmBuilderVisitor v;
    v.time = visitor.time;
    v.phase = visitor.phase;
    if (sampleInput(index, v))
        return v.data.fvec[0];
    else
        return property;
}
//--------------------------------------------------------------
std::string SignalOperation::name() const
{
    return "None";
}
//--------------------------------------------------------------
bool SignalOperation::sample(size_t index, BaseOperationVisitor& visitor)
{
    return sample(index, dynamic_cast<qb::PcmBuilderVisitor&>(visitor));
}
//--------------------------------------------------------------
bool SignalOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // to implement by children
    return false;
}
void SignalOperation::uiPreview()
{
    ImGui::Text("Preview:");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    
    ImGui::Separator();
    uiDebugIo();
    ImGui::Separator();
}
//--------------------------------------------------------------
void SignalOperation::onInputConnectionChanged()
{
    dirty();
}
//--------------------------------------------------------------
void SignalOperation::onPropertiesChanged()
{
    dirty(false);
}
//--------------------------------------------------------------
void SignalOperation::dirty(bool /*recompile*/)
{
    preview.dirty();
    for(auto& output : outputs)
    {
        for(auto& ref : output.refs)
            ref.operation->onInputConnectionChanged();
    }
}