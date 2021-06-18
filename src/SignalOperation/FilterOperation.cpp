#include "SignalOperation/FilterOperation.hpp"

#include "imgui.h"

//--------------------------------------------------------------
FreqFilter::FreqFilter()
{
    makeInput("freq", BaseOperationDataType::Float);
    makeInput("ampl", BaseOperationDataType::Float);
    makeInput("factor", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("offset",BaseOperationDataType::Float, &offset);
    makeProperty("length",BaseOperationDataType::Float, &length);
    makeProperty("minGain",BaseOperationDataType::Float, &minGain);
    makeProperty("maxGain",BaseOperationDataType::Float, &maxGain);
    makeProperty("factor",BaseOperationDataType::Float, &factor);
}
//--------------------------------------------------------------
bool FreqFilter::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output1  = getOutput(0);
    auto output2  = getOutput(1);

    float freq = inputOrProperty(0, visitor, 440.0);
    float ampl = inputOrProperty(1, visitor, 1.0);
    float fct = inputOrProperty(2, visitor, factor);
    
    auto min = [](float f1, float f2) {return f1<f2?f1:f2;};

    float gain = 0.f;
    if (length > 0.0)
        gain = 1.f - min(1.f, std::abs(freq - offset) / length);

    if (index == 1)
    {
        data.type = output2->type;
        float res = ampl + fct * (minGain + gain * (maxGain-minGain));
        data.fvec[0] = res > 1.f ? 1.f : (res < 0.f ? 0.f : res);
    }
    else
    {
        data.type = output1->type;
        data.fvec[0] = freq;
    }
    return true;
}

void FreqFilter::uiProperties()
{
    if (ImGui::InputFloat("offset", &offset)) dirty();
    if (ImGui::InputFloat("length", &length)) dirty();
    if (ImGui::InputFloat("minGain", &minGain)) dirty();
    if (ImGui::InputFloat("maxGain", &maxGain)) dirty();
    if (ImGui::InputFloat("factor", &factor)) dirty();
    
    ImGui::Separator();
    ImGui::Text("Preview");
    startSamplingGraph();
    std::array<float, 100> buf;
    for(size_t i=0; i<100; ++i)
    {
        qb::PcmBuilderVisitor visitor;
        visitor.time.duration = 1.f;
        visitor.time.t = (float)i/100.0f;
        visitor.time.sec = (float)i/100.0f;
        visitor.time.elapsed = 0.01f;
        // time.dstOp = this;

        sample(1, visitor);
        buf[i] = visitor.data.fvec[0];
    }
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}