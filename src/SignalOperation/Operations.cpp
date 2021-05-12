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
    makeOutput("value", DataType_Float);
    makeProperty({"value", DataType_Float, &value});
}
//--------------------------------------------------------------
OperationData FloatInput::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    data.type = DataType_Float;
    data.count = 1;
    data.fvec[0] = value;
    return data;
}

//--------------------------------------------------------------
NoiseInput::NoiseInput()
{
    makeOutput("value", DataType_Float);
}
//--------------------------------------------------------------
OperationData NoiseInput::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    data.type = DataType_Float;
    data.count = 1;
    data.fvec[0] = qb::noiseValue() * 2.f - 1.f;;
    return data;
}


//--------------------------------------------------------------
Repeater::Repeater()
{
    makeInput("value", DataType_Float);
    makeInput("count", DataType_Int);
    makeOutput("value", DataType_Float);
    makeProperty({"count", DataType_Int, &count});
}
//--------------------------------------------------------------
OperationData Repeater::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData b = sampleInput(1, t);

    float qu = b.type == DataType_Int ? b.ivec[0] : (float)count;
    if (qu==0.0f) qu=1.0f;

    Time t2 = t;
    t2.t = (t.t*qu);
    while(t2.t > 1.0f) t2.t -= 1.0f;

    OperationData a = sampleInput(0, t2);

    if (a.type == DataType_Float)
    {
        data.type = output->type;
        data.count = output->count;
        data.fvec[0] = a.fvec[0];
    }
    else
    {
        data.type = DataType_Error;
    }

    return data;
}


//--------------------------------------------------------------
CubicSampler::CubicSampler()
{
    makeInput("value", DataType_Float);
    makeInput("speed", DataType_Float);
    makeInput("acc", DataType_Float);
    makeInput("jerk", DataType_Float);
    makeInput("reset", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"value", DataType_Float, &value});
    makeProperty({"speed", DataType_Float, &speed});
    makeProperty({"acc", DataType_Float, &acc});
    makeProperty({"jerk", DataType_Float, &jerk});
    makeProperty({"reset", DataType_Float, &reset});
}
//--------------------------------------------------------------
OperationData CubicSampler::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);
    OperationData d = sampleInput(3, t);
    OperationData e = sampleInput(4, t);

    float va = a.type != DataType_Error ? a.fvec[0] : value;
    float sp = b.type != DataType_Error ? b.fvec[0] : speed;
    float ac = c.type != DataType_Error ? c.fvec[0] : acc;
    float je = d.type != DataType_Error ? d.fvec[0] : jerk;
    float rs = e.type != DataType_Error ? e.fvec[0] : reset;

    auto sampler = [va,sp,ac,je,rs](float t)
    {
        if(rs != 0.0f)
        {
            while(t>=rs) t -= rs;
        }
        return va + (sp + (ac + je * t) * t) * t;
    };

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = sampler(t.t);
    return data;
}

//--------------------------------------------------------------
PolynomialSampler::PolynomialSampler()
{
    _hasCustomData = true;
    makeOutput("value", DataType_Float);
    makeProperty({"count", DataType_Int, &count});
    makeProperty({"reset", DataType_Float, &reset});
    coefs.resize(1, 0);
}
//--------------------------------------------------------------
OperationData PolynomialSampler::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);

    float x = t.t;
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
    data.count = output->count;
    data.fvec[0] = res;
    return data;
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
        preview.dirty();
    }

    if (ImGui::InputFloat("reset", &reset)) preview.dirty();
    
    ImGui::Text("Coefs");
    ImGui::Separator();
    int index = 0;
    for(auto& coef : coefs)
    {
        std::string textid = std::string("##") + std::to_string(index);
        if (ImGui::InputFloat(textid.c_str(), &coef)) preview.dirty();
        index++;
    }
}

//--------------------------------------------------------------
AddOperation::AddOperation()
{
    makeInput("input1", DataType_Float);
    makeInput("input2", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
}
//--------------------------------------------------------------
OperationData AddOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 + i2;
    return data;
}

//--------------------------------------------------------------
SubOperation::SubOperation()
{
    makeInput("input1", DataType_Float);
    makeInput("input2", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
}
//--------------------------------------------------------------
OperationData SubOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 - i2;
    return data;
}

//--------------------------------------------------------------
MultOperation::MultOperation()
{
    makeInput("input1", DataType_Float);
    makeInput("input2", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
}
//--------------------------------------------------------------
OperationData MultOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1 * i2;
    return data;
}

//--------------------------------------------------------------
DivOperation::DivOperation()
{
    makeInput("input1", DataType_Float);
    makeInput("input2", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"input2", DataType_Float, &input2});
}
//--------------------------------------------------------------
OperationData DivOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float i2 = b.type == DataType_Float ? b.fvec[0] : input2;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i2!=0.0 ? i1/i2 : i1;
    return data;
}

//--------------------------------------------------------------
ClampOperation::ClampOperation()
{
    makeInput("input1", DataType_Float);
    makeInput("minVal", DataType_Float);
    makeInput("maxVal", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"input1", DataType_Float, &input1});
    makeProperty({"minVal", DataType_Float, &minVal});
    makeProperty({"maxVal", DataType_Float, &maxVal});
}
//--------------------------------------------------------------
OperationData ClampOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);
    OperationData b = sampleInput(1, t);
    OperationData c = sampleInput(2, t);

    float i1 = a.type == DataType_Float ? a.fvec[0] : input1;
    float e1 = b.type == DataType_Float ? b.fvec[0] : minVal;
    float e2 = c.type == DataType_Float ? c.fvec[0] : maxVal;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = i1>e2 ? e2 : (i1<e1 ? e1 : i1);
    return data;
}

//--------------------------------------------------------------
AbsOperation::AbsOperation()
{
    makeInput("value", DataType_Float);
    makeOutput("value", DataType_Float);
}
//--------------------------------------------------------------
OperationData AbsOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, t);

    float val = a.type == DataType_Float ? a.fvec[0] : 0.f;

    data.type = output->type;
    data.count = output->count;
    data.fvec[0] = val>0.0 ? val : -val;
    return data;
}
//--------------------------------------------------------------
TimeScale::TimeScale()
{
    makeInput("value", DataType_Float);
    makeOutput("value", DataType_Float);
    makeProperty({"delay", DataType_Float, &delay});
    makeProperty({"scale", DataType_Float, &scale});
}
//--------------------------------------------------------------
OperationData TimeScale::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    Time time2 = t;
    time2.duration = t.duration * scale;
    time2.elapsed = t.elapsed * scale;
    time2.t = t.t*scale - delay;
    time2.sec =  time2.t * time2.duration;
    time2.dstOp = this;

    OperationData data;
    auto output  = getOutput(0);
    OperationData a = sampleInput(0, time2);

    data.fvec[0] = a.type == DataType_Float ? a.fvec[0] : 0.f;
    data.type = output->type;
    data.count = output->count;
    return data;
}

//--------------------------------------------------------------
OutputOperation::OutputOperation()
{
    makeInput("signal", DataType_Float);
    makeProperty({"range", DataType_Float, &range});
    makeProperty({"duration", DataType_Float, &duration});
    makeProperty({"sample-rate", DataType_Int, &sampleRate});
    makeProperty({"sample-bits", DataType_Int, &sampleBits});

    if (defaultOutput == nullptr) defaultOutput = this;
}
OutputOperation::~OutputOperation()
{
    if (defaultOutput == this) defaultOutput = nullptr;
}
//--------------------------------------------------------------
OperationData OutputOperation::sample(size_t index, const Time& t)
{
    t.dstOp = this;
    return sampleInput(index, t);
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
    
    SignalOperation::Time time;
    time.duration = duration;
    time.elapsed = sample_t;
    time.dstOp = this;
    for(unsigned i=0;i<pcm.count();++i)
    {
        time.sec = (float)i / (float)sampleRate;
        time.t = time.sec / time.duration;
        pcm.set(i, sample(0, time).fvec[0]);
    }
}