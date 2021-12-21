#include "App/BaseAttributes.hpp"

#include "imgui.h"


//--------------------------------------------------------------
size_t BaseAttributes::count() const
{
    return wrappers.size();
}
BaseAttributes::Type BaseAttributes::typeAt(size_t i) const
{
    if(i >= 0 && i < wrappers.size())
        return std::get<1>(wrappers[i]);
    return Type::Error;
}
std::string BaseAttributes::nameAt(size_t i) const
{
    if(i >= 0 && i < wrappers.size())
        return std::get<0>(wrappers[i]);
    return "None";
}
size_t BaseAttributes::inputCount() const
{
    return inputs.size();
}
BaseAttributes::IoType BaseAttributes::inputType(size_t i) const
{
    return inputs[i].second;
}
std::string BaseAttributes::inputName(size_t i) const
{
    return inputs[i].first;
}
size_t BaseAttributes::outputCount() const
{
    return outputs.size();
}
BaseAttributes::IoType BaseAttributes::outputType(size_t i) const
{
    return outputs[i].second;
}
std::string BaseAttributes::outputName(size_t i) const
{
    return outputs[i].first;
}
void BaseAttributes::get(size_t i, std::string& value) const
{
    value = as<std::string>(wrappers[i]);
}
void BaseAttributes::get(size_t i, float& value) const
{
    value = as<float>(wrappers[i]);
}
void BaseAttributes::get(size_t i, int& value) const
{
    value = as<int>(wrappers[i]);
}
void BaseAttributes::get(size_t i, float* value) const
{
    if (typeAt(i) == Type::Color3)
    {
        auto ptr = reinterpret_cast<float*>(std::get<2>(wrappers[i]));
        for(int i=0; i<3; ++i) value[i] = ptr[i];
    }
}
void BaseAttributes::getMinMax(size_t i, float& minVal, float& maxVal) const
{
    auto minMaxVal = std::get<3>(wrappers[i]);
    minVal = minMaxVal.f2[0];
    maxVal = minMaxVal.f2[1];
}
void BaseAttributes::getMinMax(size_t i, int& minVal, int& maxVal) const
{
    auto minMaxVal = std::get<3>(wrappers[i]);
    minVal = minMaxVal.i2[0];
    maxVal = minMaxVal.i2[1];
}
void BaseAttributes::get(size_t i, bool& value) const
{
    value = as<bool>(wrappers[i]);
}
void BaseAttributes::set(size_t i, const std::string& value)
{
    as<std::string>(wrappers[i]) = value;
}
void BaseAttributes::set(size_t i, float value)
{
    as<float>(wrappers[i]) = value;
}
void BaseAttributes::set(size_t i, int value)
{
    as<int>(wrappers[i]) = value;
}
void BaseAttributes::set(size_t i, bool value)
{
    as<bool>(wrappers[i]) = value;
}
void BaseAttributes::set(size_t i, const float* value)
{
    if (typeAt(i) == Type::Color3)
    {
        auto ptr = reinterpret_cast<float*>(std::get<2>(wrappers[i]));
        for(int i=0; i<3; ++i) ptr[i] = value[i];
    }
}
void BaseAttributes::add(const std::string& name, Type type, void* ptr)
{
    wrappers.push_back({name, type, ptr, {0,0}});
}
void BaseAttributes::add(const std::string& name, float* ptr, float minVal, float maxVal)
{
    MinMaxVal mnmx; mnmx.f2[0] = minVal; mnmx.f2[1] = maxVal;
    wrappers.push_back({name, Type::Float, ptr, mnmx});
}
void BaseAttributes::add(const std::string& name, int* ptr, int minVal, int maxVal)
{
    MinMaxVal mnmx; mnmx.i2[0] = minVal; mnmx.i2[1] = maxVal;
    wrappers.push_back({name, Type::Int, ptr, mnmx});
}
void BaseAttributes::addInput(const std::string& name, IoType type)
{
    inputs.push_back(std::make_pair(name, type));
}
void BaseAttributes::addOutput(const std::string& name, IoType type)
{
    outputs.push_back(std::make_pair(name, type));
}
void BaseAttributes::ui(int index)
{
    auto type = typeAt(index);
    auto name = nameAt(index);
    bool changed = false;

    if (type == Type::Float)
    {
        float minVal = 0.0f, maxVal = 0.0f;
        getMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputFloat(name.c_str(), (float*)std::get<2>(wrappers[index]));
        else
            changed = ImGui::SliderFloat(name.c_str(), (float*)std::get<2>(wrappers[index]), minVal, maxVal);
    }
    else if (type == Type::Int)
    {
        int minVal = 0, maxVal = 0;
        getMinMax(index, minVal, maxVal);
        if (minVal == maxVal)
            changed = ImGui::InputInt(name.c_str(), (int*)std::get<2>(wrappers[index]));
        else
            changed = ImGui::SliderInt(name.c_str(), (int*)std::get<2>(wrappers[index]), minVal, maxVal);
    }
    else if (type == Type::Bool)
    {
        changed = ImGui::Checkbox(name.c_str(), (bool*)std::get<2>(wrappers[index]));
    }
    else if (type == Type::Color3)
    {
        changed = ImGui::ColorEdit3(name.c_str(), (float*)std::get<2>(wrappers[index]));
    }

    if (changed) onChanged();
}
void BaseAttributes::ui()
{
    ImGui::BeginChild("##properties");
    for(size_t i=0; i<count(); ++i) ui((int)i);
    ImGui::EndChild();
}
bool BaseAttributes::hasCustomData() const
{
    return _hasCustomData;
}
void BaseAttributes::saveCustomData(JsonValue& json)
{

}
void BaseAttributes::loadCustomData(JsonValue& json)
{

}
