#include "gui/nodal/SignalOperationNode.hpp"

#include "imgui.h"

namespace qb
{
    namespace imgui
    {
        static float s_sampler_count = 100;
    }
}

//--------------------------------------------------------------
SignalOperationNode::SignalOperationNode(const std::string& title, size_t nodeTypeId)
    : UiNode(title, vec2(0.0,0.0), vec2(70,70))
    , _nodetypeId(nodeTypeId)
{
}

//--------------------------------------------------------------
void SignalOperationNode::setOperation(SignalOperation* op)
{
    _operation = op;
}

//--------------------------------------------------------------
size_t SignalOperationNode::nodeTypeId() const
{
    return _nodetypeId;
}

//--------------------------------------------------------------
SignalOperation* SignalOperationNode::getOperation()
{
    return _operation;
}

void SignalOperationNode::displayPreview()
{
    ImGui::Separator();
    ImGui::Text("Preview");
    getOperation()->validateGraph();
    std::array<float, 100> buf;
    s_imgui_sampler_set_count(100);
    for(int i=0; i<100; ++i) buf[i] = s_imgui_sampler(getOperation(), i);
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}

void SignalOperationNode::s_imgui_sampler_set_count(int count)
{
    qb::imgui::s_sampler_count = (float)count;
}

//--------------------------------------------------------------
float SignalOperationNode::s_imgui_sampler(void* data, int idx)
{
    SignalOperation& op = *(SignalOperation*)data;
    SignalOperation::Time time;
    time.duration = 1.0;
    time.t = (float)idx/qb::imgui::s_sampler_count;
    time.sec = (float)idx/qb::imgui::s_sampler_count;
    time.elapsed = 0.01f;
    time.dstOp = &op;
    return op.sample(0, time).fvec[0];
}

void SignalOperationNode::drawPreview(const Rect& previewArea)
{
    const int sample_count = (int)_preview.size();
    RenderInterface::setColor(0x777777FF);
    Rect target = previewArea + parentPosition + position;
    RenderInterface::fill(target.p0, target.p1);
    vec2 barS(previewArea.size().x / (float)sample_count, 0);
    RenderInterface::setColor(0xEEEEEEFF);

    if(_hasChange)
    {
        getOperation()->validateGraph();
        float minVal = 100000.0; float maxVal = -100000.0;
        s_imgui_sampler_set_count(sample_count);
        for(size_t i=0; i<sample_count; ++i)
        {
            _preview[i] = s_imgui_sampler(getOperation(), (int)i);
            if (_preview[i] < minVal) minVal = _preview[i];
            if (_preview[i] > maxVal) maxVal = _preview[i];
        }
        _previewMaxVal = abs(maxVal) > abs(minVal) ? abs(maxVal) : abs(minVal);
        _hasChange = false;
    }

    for(int i=0; i< sample_count; ++i)
    {
        vec2 p = target.p0 + vec2((float)i*barS.x,previewArea.size().y*0.5f);
        float y = _preview[i] / _previewMaxVal;
        float h = previewArea.size().y*0.5f * -y;
        RenderInterface::fill(p, p+vec2(barS.x,h));
    }
}

void SignalOperationNode::dirtyPreview()
{
    _hasChange = true;
}