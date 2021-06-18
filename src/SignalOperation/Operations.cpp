#include "SignalOperation/Operations.hpp"

#include <iostream>
#include <string>

#include "imgui.h"
#include "App/AppInterface.hpp"

#include "Json/Json.hpp"

#include "Core/PseudoRand.hpp"

OutputOperation* OutputOperation::defaultOutput = nullptr;

//--------------------------------------------------------------
FloatInput::FloatInput()
{
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("value", BaseOperationDataType::Float, &value);
}
//--------------------------------------------------------------
bool FloatInput::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    visitor.data.type = BaseOperationDataType::Float;
    visitor.data.fvec[0] = value;
    return true;
}

//--------------------------------------------------------------
NoiseInput::NoiseInput()
{
    makeOutput("value", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool NoiseInput::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    data.type = BaseOperationDataType::Float;
    data.fvec[0] = qb::noiseValue() * 2.f - 1.f;;
    return true;
}


//--------------------------------------------------------------
Repeater::Repeater()
{
    makeInput("value", BaseOperationDataType::Float);
    makeInput("count", BaseOperationDataType::Int);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("count", BaseOperationDataType::Int, &count);
}
//--------------------------------------------------------------
bool Repeater::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);


    float qu = inputOrProperty(1, visitor, (float)count);
    if (qu==0.0f) qu=1.0f;

    qb::PcmBuilderVisitor v2;
    v2.phase = visitor.phase;
    qb::Time& t2 = v2.time;
    auto& t = visitor.time;
    t2.t = (t.t*qu);
    while(t2.t > 1.0f) t2.t -= 1.0f;

    data.type = output->type;
    data.fvec[0] = inputOrProperty(0, v2, 0.0);

    return true;
}


//--------------------------------------------------------------
CubicSampler::CubicSampler()
{
    makeInput("value", BaseOperationDataType::Float);
    makeInput("speed", BaseOperationDataType::Float);
    makeInput("acc", BaseOperationDataType::Float);
    makeInput("jerk", BaseOperationDataType::Float);
    makeInput("reset", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("value", BaseOperationDataType::Float, &value);
    makeProperty("speed", BaseOperationDataType::Float, &speed);
    makeProperty("acc", BaseOperationDataType::Float, &acc);
    makeProperty("jerk", BaseOperationDataType::Float, &jerk);
    makeProperty("reset", BaseOperationDataType::Float, &reset);
}
//--------------------------------------------------------------
bool CubicSampler::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    if(reset < 0.0f) reset = 0.0f;

    float va = inputOrProperty(0, visitor, value);
    float sp = inputOrProperty(1, visitor, speed);
    float ac = inputOrProperty(2, visitor, acc);
    float je = inputOrProperty(3, visitor, jerk);
    float rs = inputOrProperty(4, visitor, reset);

    auto sampler = [va,sp,ac,je,rs](float t)
    {
        if(rs != 0.0f)
        {
            while(t>=rs) t -= rs;
        }
        return va + (sp + (ac + je * t) * t) * t;
    };

    data.type = output->type;
    data.fvec[0] = sampler(visitor.time.t);
    return true;
}

//--------------------------------------------------------------
PolynomialSampler::PolynomialSampler()
{
    _hasCustomData = true;
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("count", BaseOperationDataType::Int, &count);
    makeProperty("reset", BaseOperationDataType::Float, &reset);
    coefs.resize(1, 0);
}
//--------------------------------------------------------------
bool PolynomialSampler::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);

    if(reset < 0.0f) reset = 0.0f;

    float x = visitor.time.t;
    if(reset > 0.0)
    {
        while(x>=reset) x -= reset;
    }
    float res = 0.0;
    float ve = 1.0;
    for(auto coef : coefs)
    {
        res += coef * ve;
        ve *= x;
    }

    data.type = output->type;
    data.fvec[0] = res;
    return true;
}

void PolynomialSampler::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("coefs");
    int index = 0;
    for (auto coef : coefs)
    {
        jArray.setPath(index++).set(coef);
    }
}
void PolynomialSampler::loadCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("coefs");
    count = (int)jArray.count();
    coefs.resize(jArray.count());
    int index = 0;
    for(auto& jcoef : jArray.array.values)
    {
        coefs[index++] = (float)jcoef.getNumeric();
    }
}

void PolynomialSampler::uiProperties()
{
    ImGui::Text("Preview:");
    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    ImGui::Separator();

    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        coefs.resize(count);
        dirty();
    }

    if (ImGui::InputFloat("reset", &reset)) dirty();
    
    ImGui::Text("Coefs");
    ImGui::Separator();
    int index = 0;
    for(auto& coef : coefs)
    {
        std::string textid = std::string("##") + std::to_string(index);
        if (ImGui::InputFloat(textid.c_str(), &coef)) dirty();
        index++;
    }
}

//--------------------------------------------------------------
AddOperation::AddOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("input2", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1", BaseOperationDataType::Float, &input1);
    makeProperty("input2", BaseOperationDataType::Float, &input2);
}
//--------------------------------------------------------------
bool AddOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float i1 = inputOrProperty(0, visitor, input1);
    float i2 = inputOrProperty(1, visitor, input2);

    data.type = output->type;
    data.fvec[0] = i1 + i2;
    return true;
}

//--------------------------------------------------------------
SubOperation::SubOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("input2", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1", BaseOperationDataType::Float, &input1);
    makeProperty("input2", BaseOperationDataType::Float, &input2);
}
//--------------------------------------------------------------
bool SubOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float i1 = inputOrProperty(0, visitor, input1);
    float i2 = inputOrProperty(1, visitor, input2);

    data.type = output->type;
    data.fvec[0] = i1 - i2;
    return true;
}

//--------------------------------------------------------------
MultOperation::MultOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("input2", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1", BaseOperationDataType::Float, &input1);
    makeProperty("input2", BaseOperationDataType::Float, &input2);
}
//--------------------------------------------------------------
bool MultOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float i1 = inputOrProperty(0, visitor, input1);
    float i2 = inputOrProperty(1, visitor, input2);

    data.type = output->type;
    data.fvec[0] = i1 * i2;
    return true;
}

//--------------------------------------------------------------
DivOperation::DivOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("input2", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1", BaseOperationDataType::Float, &input1);
    makeProperty("input2", BaseOperationDataType::Float, &input2);
}
//--------------------------------------------------------------
bool DivOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float i1 = inputOrProperty(0, visitor, input1);
    float i2 = inputOrProperty(1, visitor, input2);

    data.type = output->type;
    data.fvec[0] = i2!=0.0 ? i1/i2 : i1;
    return true;
}

//--------------------------------------------------------------
ClampOperation::ClampOperation()
{
    makeInput("input1", BaseOperationDataType::Float);
    makeInput("minVal", BaseOperationDataType::Float);
    makeInput("maxVal", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("input1", BaseOperationDataType::Float, &input1);
    makeProperty("minVal", BaseOperationDataType::Float, &minVal);
    makeProperty("maxVal", BaseOperationDataType::Float, &maxVal);
}
//--------------------------------------------------------------
bool ClampOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float i1 = inputOrProperty(0, visitor, input1);
    float e1 = inputOrProperty(1, visitor, minVal);
    float e2 = inputOrProperty(2, visitor, maxVal);
    
    data.type = output->type;
    data.fvec[0] = i1>e2 ? e2 : (i1<e1 ? e1 : i1);
    return true;
}

//--------------------------------------------------------------
AbsOperation::AbsOperation()
{
    makeInput("value", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool AbsOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    float val = inputOrProperty(0, visitor, 0.0);

    data.type = output->type;
    data.fvec[0] = val>0.0 ? val : -val;
    return true;
}
//--------------------------------------------------------------
TimeScale::TimeScale()
{
    makeInput("value", BaseOperationDataType::Float);
    makeOutput("value", BaseOperationDataType::Float);
    makeProperty("delay", BaseOperationDataType::Float, &delay);
    makeProperty("scale", BaseOperationDataType::Float, &scale);
}
//--------------------------------------------------------------
bool TimeScale::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    qb::PcmBuilderVisitor v2;
    v2.time = visitor.time;
    v2.time.duration = visitor.time.duration * scale;
    v2.time.elapsed = visitor.time.elapsed * scale;
    v2.time.t = visitor.time.t*scale - delay;
    v2.time.sec =  v2.time.t * v2.time.duration;
    // time2.dstOp = this;

    qb::OperationData& data = visitor.data;
    auto output  = getOutput(0);
    data.fvec[0] = inputOrProperty(0, v2, 0.0);
    data.type = output->type;
    return true;
}

//--------------------------------------------------------------
OutputOperation::OutputOperation()
{
    makeInput("signal", BaseOperationDataType::Float);
    makeProperty("range", BaseOperationDataType::Float, &range);
    makeProperty("duration", BaseOperationDataType::Float, &duration);
    makeProperty("sample-rate", BaseOperationDataType::Int, &sampleRate);
    makeProperty("sample-bits", BaseOperationDataType::Int, &sampleBits);

    if (defaultOutput == nullptr) defaultOutput = this;
}
OutputOperation::~OutputOperation()
{
    if (defaultOutput == this) defaultOutput = nullptr;
}
//--------------------------------------------------------------
bool OutputOperation::sample(size_t index, qb::PcmBuilderVisitor& visitor)
{
    // t.dstOp = this;
    return sampleInput(index, visitor);
}

void OutputOperation::uiProperties()
{
    static constexpr size_t sampleRateCount = 6;
    static constexpr size_t sampleFormatCount = 2;
    
    static constexpr std::array<int,sampleRateCount> indexedRates = {AudioSettings::SampleRate_8kHz, AudioSettings::SampleRate_11kHz, AudioSettings::SampleRate_22kHz, AudioSettings::SampleRate_32kHz, AudioSettings::SampleRate_44kHz, AudioSettings::SampleRate_48kHz};
    static constexpr std::array<const char*,sampleRateCount> sampleRateNames = {"SampleRate_8kHz", "SampleRate_11kHz", "SampleRate_22kHz", "SampleRate_32kHz", "SampleRate_44kHz", "SampleRate_48kHz"};
    static constexpr std::array<int,sampleFormatCount> indexedFormats = {AudioSettings::Format_Mono8, AudioSettings::Format_Mono16};
    static constexpr std::array<const char*,sampleFormatCount> sampleFormatNames = {"Format_Mono8", "Format_Mono16"};

    if (defaultOutput == this)
    {
        ImGui::Text("Default output");
    }
    else if (ImGui::Button("Set as default output"))
    {
        defaultOutput = this;
    }

    preview.compute(this);
    ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, -range, range, ImVec2(0, 60.0f));

    if (ImGui::InputFloat("range", &range)) preview.dirty();
    ImGui::InputFloat("duration scale", &duration);

    if (sampleRateIndex == -1)
    {
        if(sampleRate == 0) sampleRate = AudioSettings::SampleRate_44kHz;
        sampleRateIndex = 4;
        for(size_t i=0; i<sampleRateCount; ++i)
        {
            if (sampleRate == indexedRates[i])
            {
                sampleRateIndex = (int)i;
                break;
            }
        }
    }
    
    if (sampleFormatIndex == -1)
    {
        if(sampleBits == 0) sampleBits = AudioSettings::Format_Mono16;
        sampleFormatIndex = 1;
        for(size_t i=0; i<sampleFormatCount; ++i)
        {
            if (sampleBits == indexedFormats[i])
            {
                sampleFormatIndex = (int)i;
                break;
            }
        }
    }

    if (ImGui::Button(sampleRateNames[sampleRateIndex]))
    {
        sampleRateIndex = (sampleRateIndex+1) % (int)sampleRateCount;
        sampleRate = indexedRates[sampleRateIndex];
    }
    
    if (ImGui::Button(sampleFormatNames[sampleFormatIndex]))
    {
        sampleFormatIndex = (sampleFormatIndex+1) % (int)sampleFormatCount;
        sampleBits = indexedFormats[sampleFormatIndex];
    }

    if (ImGui::Button("Play"))
    {
        auto& sound = *SoundNode::getDefault();
        if(sound.getState() != SoundNode::Playing)
        {
            std::unique_ptr<PcmDataBase> pcm;
            if (sampleBits == AudioSettings::Format_Mono8 || sampleBits == AudioSettings::Format_Stereo8)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
            if (sampleBits == AudioSettings::Format_Mono16 || sampleBits == AudioSettings::Format_Stereo16)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
            generate(*pcm);
            sound.queue(*pcm.get());
            sound.play();
        }
    }
}
//--------------------------------------------------------------
void OutputOperation::generate(PcmDataBase& pcm)
{
    if (pcm.format != sampleBits)
        std::cout << "warning: AudioOutputNode::generate() pcm format does not match" << std::endl;
    if (duration == 0.0f)
    {
        std::cout << "error: duration = 0; 1s is used" << std::endl;
        duration = 1.0;
    }

    startSamplingGraph();

    pcm.sampleRate = (AudioSettings::SampleRate)sampleRate;
    pcm.resize((size_t)(duration * sampleRate));
    
    float sample_t = 1.f / (float)sampleRate;
    
    qb::PcmBuilderVisitor visitor;
    visitor.time.duration = duration;
    visitor.time.elapsed = sample_t;
    // time.dstOp = this;
    for(unsigned i=0;i<pcm.count();++i)
    {
        visitor.time.sec = (float)i / (float)sampleRate;
        visitor.time.t = visitor.time.sec / visitor.time.duration;
        sample(0, visitor);
        pcm.set(i, visitor.data.fvec[0]);
    }
}