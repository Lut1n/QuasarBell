#include "SignalOperation/FilterOperation.hpp"

#include "imgui.h"

//--------------------------------------------------------------
FreqFilter::FreqFilter()
{
    makeInput("freq", DataType_Float);
    makeInput("ampl", DataType_Float);
    makeInput("factor", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"offset",DataType_Float, &offset});
    makeProperty({"length",DataType_Float, &length});
    makeProperty({"minGain",DataType_Float, &minGain});
    makeProperty({"maxGain",DataType_Float, &maxGain});
    makeProperty({"factor",DataType_Float, &factor});
}
//--------------------------------------------------------------
OperationData FreqFilter::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output1  = getOutput(0);
    auto output2  = getOutput(1);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float freq = a.type == DataType_Float ? a.fvec[0] : 440.f;
    float ampl = b.type == DataType_Float ? b.fvec[0] : 1.f;
    float fct = c.type == DataType_Float ? c.fvec[0] : factor;
    
    auto min = [](float f1, float f2) {return f1<f2?f1:f2;};

    float gain = 0.f;
    if (length > 0.0)
        gain = 1.f - min(1.f, std::abs(freq - offset) / length);

    if (index == 1)
    {
        data.type = output2->type;
        data.count = output2->count;
        float res = ampl + fct * (minGain + gain * (maxGain-minGain));
        data.fvec[0] = res > 1.f ? 1.f : (res < 0.f ? 0.f : res);
    }
    else
    {
        data.type = output1->type;
        data.count = output1->count;
        data.fvec[0] = freq;
    }
    return data;
}

void FreqFilter::uiProperties()
{
    if (ImGui::InputFloat("offset", &offset)) preview.dirty();
    if (ImGui::InputFloat("length", &length)) preview.dirty();
    if (ImGui::InputFloat("minGain", &minGain)) preview.dirty();
    if (ImGui::InputFloat("maxGain", &maxGain)) preview.dirty();
    if (ImGui::InputFloat("factor", &factor)) preview.dirty();
    
    ImGui::Separator();
    ImGui::Text("Preview");
    startSamplingGraph();
    std::array<float, 100> buf;
    for(size_t i=0; i<100; ++i)
    {
        SignalOperation::Time time;
        time.duration = 1.f;
        time.t = (float)i/100.0f;
        time.sec = (float)i/100.0f;
        time.elapsed = 0.01f;
        time.dstOp = this;

        buf[i] = sample(1, time).fvec[0];
    }
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}