#include "SignalOperation/Waveform.hpp"

#include <cmath> // sin
#include <iostream>

#include "imgui.h"

#include "Core/PseudoRand.hpp"

//--------------------------------------------------------------
Waveform::Waveform()
{
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("period", BaseOperationDataType::Float, &period);
    makeProperty("min", BaseOperationDataType::Float, &minVal);
    makeProperty("max", BaseOperationDataType::Float, &maxVal);
    makeProperty("type", BaseOperationDataType::Int, &type);
    makeProperty("noise-seed", BaseOperationDataType::Int, &noiseSeed);
    makeProperty("noise-samples", BaseOperationDataType::Int, &noiseSamples);
}
//--------------------------------------------------------------
void Waveform::startSampling()
{
    if (type == Type::Noise) computeNoise();
}

//--------------------------------------------------------------
float Waveform::sampleFlip(float t)
{
    constexpr float pi2 = 2.0f * 3.141592f;
    if(type == Type::Sin)
    {
        return std::sin(t * pi2);
    }
    else if(type == Type::Saw)
    {
        while(t>1.0) t -= 1.0;
        if(t<0.25f)
        {
            return t/0.25f;
        }
        else if(t<0.50f)
        {
            return 1.0f - (t-0.25f)/0.25f;
        }
        else if(t<0.75f)
        {
            return -(t-0.50f)/0.25f;
        }
        else
        {
            return (t-0.75f)/0.25f - 1.0f;
        }
    }
    else if(type == Type::Square)
    {
        return std::sin(t * pi2) >0.f ? 1.f : -1.f;
    }
    else if(type == Type::Noise)
    {
        while(t>1.f) t -= 1.f;
        return samples[(size_t)(t * samples.size())];
    }
    return 0.f;
}

//--------------------------------------------------------------
bool Waveform::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    
    data.type = output->type;

    float ampl = (maxVal-minVal);
    data.fvec[0] = (sampleFlip(visitor.time.t * period) + 1.0f) * 0.5f * ampl + minVal;
    return true;
}

void Waveform::computeNoise()
{
    if (_prev_seed != noiseSeed || _prev_samples != noiseSamples)
    {
        _prev_samples = noiseSamples;
        _prev_seed = noiseSeed;

        qb::noiseSeed(noiseSeed);
        samples.resize(noiseSamples);
        for(auto& s : samples) s = qb::noiseValue() * 2.f - 1.f;
    }
}

void Waveform::uiProperties()
{
    ImGui::Text("Preview:");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    ImGui::Separator();

    if (ImGui::InputFloat("period", &period)) dirty();
    if (ImGui::InputFloat("min", &minVal)) dirty();
    if (ImGui::InputFloat("max", &maxVal)) dirty();

    constexpr size_t count = 4;
    constexpr std::array<const char*, count> typeNames = {"sin", "saw", "square", "noise"};
    if (ImGui::Button(typeNames[type]))
    {
        type = (type+1) % count;
        dirty();
    }

    if (type == 3)
    {
        if (ImGui::InputInt("noise seed", &noiseSeed)) dirty();
        if (ImGui::InputInt("noise size", &noiseSamples)) dirty();
    }
}