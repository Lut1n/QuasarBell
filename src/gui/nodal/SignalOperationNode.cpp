#include "gui/nodal/SignalOperationNode.hpp"

#include "imgui.h"

//--------------------------------------------------------------
SignalOperationNode::SignalOperationNode(const std::string& title, const vec2& position)
    : UiNode(title, position, vec2(60,60))
{
}

//--------------------------------------------------------------
void SignalOperationNode::setOperation(SignalOperation* op)
{
    operation = op;
}

//--------------------------------------------------------------
SignalOperation* SignalOperationNode::getOperation()
{
    return operation;
}

void SignalOperationNode::displayPreview()
{
    ImGui::Separator();
    ImGui::Text("Preview");
    getOperation()->validateGraph();
    ImGui::PlotLines("##preview", s_imgui_sampler, getOperation(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
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