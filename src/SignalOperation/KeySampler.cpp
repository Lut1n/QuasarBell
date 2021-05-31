#include "SignalOperation/KeySampler.hpp"

#include <iostream>
#include <string>

#include "imgui.h"

#include "Json/Json.hpp"

//--------------------------------------------------------------
KeySampler::KeySampler()
{
    _hasCustomData = true;
    makeOutput("value", DataType_Float);
    makeProperty({"count", DataType_Int, &count});
    makeProperty({"interpo", DataType_Int, &interpo});
    keys.resize(1,{0.f,1.f});
}
//--------------------------------------------------------------
OperationData KeySampler::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);

    data.type = output->type;
    data.count = output->count;

    int index1 = -1;
    int index2 = -1;
    for(size_t i=0; i<keys.size(); ++i)
    {
        int j = (int)(keys.size()-1-i);
        if(keys[i].first <= t.t) index1 = (int)i;
        if(keys[j].first >= t.t) index2 = (int)j;
    }

    if(index1 == -1 && index2 == -1)
        data.fvec[0] = 0.0;
    else if(index1 == -1)
        data.fvec[0] = keys[index2].second;
    else if(index2 == -1)
        data.fvec[0] = keys[index1].second;
    else if(index1 == index2)
        data.fvec[0] = keys[index1].second;
    else
    {
        float x = (t.t-keys[index1].first) / (keys[index2].first-keys[index1].first);
        data.fvec[0] = keys[index1].second + interpolate(x) * (keys[index2].second-keys[index1].second);
    }
    
    return data;
}
//--------------------------------------------------------------
float KeySampler::interpolate(float x)
{
    if (interpo == Interpo::Flat)
        return 0;
    else if (interpo == Interpo::Linear)
        return x;
    else if (interpo == Interpo::Cubic)
        return x * x * (3.f - 2.f * x);
    return x;
}
//--------------------------------------------------------------
void KeySampler::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("key-values");
    int index = 0;
    for (auto kv : keys)
    {
        jArray.setPath(index, 0).set(kv.first);
        jArray.setPath(index, 1).set(kv.second);
        index++;
    }
}
//--------------------------------------------------------------
void KeySampler::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("key-values");
    count = (int)jArray.count();
    keys.resize(jArray.count());
    int index = 0;
    for(auto& jkv : jArray.array.values)
    {
        keys[index] = {(float)jkv.setPath(0).getNumeric(),(float) jkv.setPath(1).getNumeric()};
        index++;
    }
}

//--------------------------------------------------------------
void KeySampler::uiProperties()
{
    constexpr size_t typeCount = 3;
    constexpr std::array<const char*, typeCount> typeNames = {"flat", "linear", "cubic"};
    if (ImGui::Button(typeNames[interpo]))
    {
        interpo = (interpo+1) % typeCount;
        dirty();
    }

    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        keys.resize(count);
        dirty();
    }
    
    ImGui::Columns(2);
    ImGui::Text("Time");
    ImGui::NextColumn();
    ImGui::Text("Value");
    ImGui::NextColumn();
    ImGui::Separator();
    int index = 0;
    for(auto& kv : keys)
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