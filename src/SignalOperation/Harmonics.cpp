#include "SignalOperation/Harmonics.hpp"

#include <cmath> // sin
#include <iostream>
#include <string>

#include "imgui.h"

#include "Json/Json.hpp"

//--------------------------------------------------------------
Harmonics::Harmonics()
{
    _hasCustomData = true;
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("count", BaseOperationDataType::Int, &count);
    freqs.resize(1,{440.f,1.f});
}

//--------------------------------------------------------------
bool Harmonics::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    auto output  = getOutput(0);

    qb::OperationData& data = visitor.data;
    data.type = output->type;
    data.fvec[0] = 0.0;
    for(auto f : freqs) data.fvec[0] += std::sin( visitor.time.t * f.first * 2.f * 3.141592f ) * f.second;
    return true;
}

//--------------------------------------------------------------
void Harmonics::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("freq-ampl");
    int index = 0;
    for (auto fa : freqs)
    {
        jArray.setPath(index, 0).set(fa.first);
        jArray.setPath(index, 1).set(fa.second);
        index++;
    }
}
//--------------------------------------------------------------
void Harmonics::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("freq-ampl");
    count = (int)jArray.count();
    freqs.resize(jArray.count());
    int index = 0;
    for(auto& jfa : jArray.array.values)
    {
        freqs[index] = { (float) jfa.setPath(0).getNumeric(),(float) jfa.setPath(1).getNumeric()};
        index++;
    }
}

void Harmonics::uiProperties()
{
    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        freqs.resize(count);
        dirty();
    }
    
    ImGui::Columns(2);
    ImGui::Text("Freq");
    ImGui::NextColumn();
    ImGui::Text("Ampl");
    ImGui::NextColumn();
    ImGui::Separator();
    int index = 0;
    for(auto& kv : freqs)
    {
        std::string keytext = std::string("##key") + std::to_string(index);
        std::string valtext = std::string("##val") + std::to_string(index);
        if (ImGui::InputFloat(keytext.c_str(), &kv.first)) dirty();
        ImGui::NextColumn();
        if (ImGui::InputFloat(valtext.c_str(), &kv.second)) dirty();
        ImGui::NextColumn();
        index++;
    }
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::Text("Preview");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}