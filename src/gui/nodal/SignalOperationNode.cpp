#include "gui/nodal/SignalOperationNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
SignalOperationNode::SignalOperationNode(const std::string& title, size_t nodeTypeId)
    : UiNode(title, vec2(0.0,0.0), vec2(60,60))
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
    for(size_t i=0; i<100; ++i) buf[i] = s_imgui_sampler(getOperation(), i);
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    // ImGui::PlotLines("##preview", s_imgui_sampler, getOperation(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}

//--------------------------------------------------------------
float SignalOperationNode::s_imgui_sampler(void* data, int idx)
{
    SignalOperation& op = *(SignalOperation*)data;
    SignalOperation::Time time;
    time.duration = 1.0;
    time.t = (float)idx/100.0f;
    time.sec = (float)idx/100.0f;
    time.elapsed = 0.01;
    return op.sample(0, time).fvec[0];
}