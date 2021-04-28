#include "gui/nodal/FilterNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "imgui.h"

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, FilterNode> filter_node_factory(qb::OperationType_Filter);


//--------------------------------------------------------------
FilterNode::FilterNode()
    : SignalOperationNode("Filter", qb::OperationType_Filter)
{
    addPin(0, "freq", true);
    addPin(1, "ampl", true);
    addPin(0, "freq", false);
    addPin(1, "ampl", false);
    addPin(2, "factor", false);
    setOperation(&filter);
}
//--------------------------------------------------------------
void FilterNode::displayProperties()
{
    if (ImGui::InputFloat("offset", &filter.offset)) dirtyPreview();
    if (ImGui::InputFloat("length", &filter.length)) dirtyPreview();
    if (ImGui::InputFloat("minGain", &filter.minGain)) dirtyPreview();
    if (ImGui::InputFloat("maxGain", &filter.maxGain)) dirtyPreview();
    if (ImGui::InputFloat("factor", &filter.factor)) dirtyPreview();
    
    ImGui::Separator();
    ImGui::Text("Preview");
    getOperation()->validateGraph();
    std::array<float, 100> buf;
    for(size_t i=0; i<100; ++i)
    {
        SignalOperation::Time time;
        time.duration = 1.0;
        time.t = (float)i/100.0f;
        time.sec = (float)i/100.0f;
        time.elapsed = 0.01;

        buf[i] =getOperation()->sample(1, time).fvec[0];
    }
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}