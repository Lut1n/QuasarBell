#include "SignalOperation/SignalAttributes.hpp"

#include "Audio/PcmData.hpp"
#include "Core/PseudoRand.hpp"
#include "App/UserFileInput.hpp"

#include "imgui.h"

#include <array>

using namespace qb;

static UserFileInput waveExportIo("wav path", ".wav", "./exported/output.wav");

//--------------------------------------------------------------
EnvelopData::EnvelopData() : BaseAttributes(TypeId)
{
    addOutput("value", IoType::Signal);
    add("attack", Type::Float, &attack);
    add("decay", Type::Float, &decay);
    add("sustain", Type::Float, &sustain);
    add("release", Type::Float, &release);
}
//--------------------------------------------------------------
FreqFilterData::FreqFilterData() : BaseAttributes(TypeId)
{
    addInput("freq", IoType::Signal);
    addInput("ampl", IoType::Signal);
    addInput("factor", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("offset",Type::Float, &offset);
    add("length",Type::Float, &length);
    add("minGain",Type::Float, &minGain);
    add("maxGain",Type::Float, &maxGain);
    add("factor",Type::Float, &factor);
}
//--------------------------------------------------------------
HarmonicsData::HarmonicsData() : BaseAttributes(TypeId)
{
    addOutput("value", IoType::Signal);
    _hasCustomData = true;
    add("count", Type::Int, &count);
    freqs.resize(1,{440.f,1.f});
}
//--------------------------------------------------------------
void HarmonicsData::saveCustomData(JsonValue& json)
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
void HarmonicsData::loadCustomData(JsonValue& json)
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
//--------------------------------------------------------------
KeySamplerData::KeySamplerData() : BaseAttributes(TypeId)
{
    _hasCustomData = true;
    addOutput("value", IoType::Signal);
    add("count", Type::Int, &count);
    add("interpo", Type::Int, &interpo);
    keys.resize(1,{0.f,1.f});
}
//--------------------------------------------------------------
void KeySamplerData::saveCustomData(JsonValue& json)
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
void KeySamplerData::loadCustomData(JsonValue& json)
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
MixData::MixData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("input2", IoType::Signal);
    addInput("delta", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1",Type::Float, &input1);
    add("input2",Type::Float, &input2);
    add("delta",&delta, 0.0f, 1.0f);
}
//--------------------------------------------------------------
FloatData::FloatData() : BaseAttributes(TypeId)
{
    addOutput("value", IoType::Signal);
    add("value", Type::Float, &value);
}
// --------------------------------------------------------------
NoiseData::NoiseData() : BaseAttributes(TypeId)
{
    addOutput("value", IoType::Signal);
}
// --------------------------------------------------------------
RepeaterData::RepeaterData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addInput("count", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("count", Type::Int, &count);
}
//--------------------------------------------------------------
CubicSamplerData::CubicSamplerData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addInput("speed", IoType::Signal);
    addInput("acc", IoType::Signal);
    addInput("jerk", IoType::Signal);
    addInput("reset", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("value", Type::Float, &value);
    add("speed", Type::Float, &speed);
    add("acc", Type::Float, &acc);
    add("jerk", Type::Float, &jerk);
    add("reset", Type::Float, &reset);
}
//--------------------------------------------------------------
PolynomialSamplerData::PolynomialSamplerData() : BaseAttributes(TypeId)
{
    _hasCustomData = true;
    addOutput("value", IoType::Signal);
    add("count", Type::Int, &count);
    add("reset", Type::Float, &reset);
    coefs.resize(1, 0);
}
void PolynomialSamplerData::saveCustomData(JsonValue& json)
{
    auto& jArray = json.setPath("coefs");
    int index = 0;
    for (auto coef : coefs)
    {
        jArray.setPath(index++).set(coef);
    }
}
void PolynomialSamplerData::loadCustomData(JsonValue& json)
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
//--------------------------------------------------------------
AddData::AddData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("input2", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1", Type::Float, &input1);
    add("input2", Type::Float, &input2);
}
//--------------------------------------------------------------
SubData::SubData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("input2", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1", Type::Float, &input1);
    add("input2", Type::Float, &input2);
}
//--------------------------------------------------------------
MultData::MultData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("input2", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1", Type::Float, &input1);
    add("input2", Type::Float, &input2);
}
//--------------------------------------------------------------
DivData::DivData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("input2", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1", Type::Float, &input1);
    add("input2", Type::Float, &input2);
}
//--------------------------------------------------------------
ClampData::ClampData() : BaseAttributes(TypeId)
{
    addInput("input1", IoType::Signal);
    addInput("minVal", IoType::Signal);
    addInput("maxVal", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("input1", Type::Float, &input1);
    add("minVal", Type::Float, &minVal);
    add("maxVal", Type::Float, &maxVal);
}
//--------------------------------------------------------------
AbsData::AbsData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addOutput("value", IoType::Signal);
}
//--------------------------------------------------------------
TimeScaleData::TimeScaleData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("delay", Type::Float, &delay);
    add("scale", Type::Float, &scale);
}
//--------------------------------------------------------------
OutputData::OutputData() : BaseAttributes(TypeId)
{
    addInput("signal", IoType::Signal);
    add("range", Type::Float, &range);
    add("duration", Type::Float, &duration);
    add("sample-rate", Type::Int, &sampleRate);
    add("sample-bits", Type::Int, &sampleBits);
}
//--------------------------------------------------------------
OscillatorData::OscillatorData() : BaseAttributes(TypeId)
{
    addInput("freq", IoType::Signal);
    addInput("ampl", IoType::Signal);
    addInput("waveform", IoType::Signal);
    addOutput("signal", IoType::Signal);
    add("freq", Type::Float, &freq);
    add("ampl", Type::Float, &ampl);
}
//--------------------------------------------------------------
PitchData::PitchData() : BaseAttributes(TypeId)
{
    addInput("octave", IoType::Signal);
    addInput("semitone", IoType::Signal);
    addOutput("freq", IoType::Signal);
    add("octave", Type::Int, &octave);
    add("semitone", Type::Int, &semitone);
}
int PitchData::getMidiIndex() const
{
    constexpr int midi_first_octave = -1;
    constexpr int midi_semitones = 12;
    return (octave-midi_first_octave) * midi_semitones + semitone;
}
//--------------------------------------------------------------
float PitchData::getFreq() const
{
    constexpr int midi_semitones = 12;
    constexpr int midi_la440 = 69;
    return pitchToFreq((float)getMidiIndex(), midi_la440, midi_semitones);
}
//--------------------------------------------------------------
QuantizerData::QuantizerData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addInput("quantity", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("quantity", Type::Int, &quantity);
}
//--------------------------------------------------------------
StepData::StepData() : BaseAttributes(TypeId)
{
    addInput("value", IoType::Signal);
    addInput("edge", IoType::Signal);
    addInput("then", IoType::Signal);
    addInput("else", IoType::Signal);
    addOutput("value", IoType::Signal);
    add("edge", Type::Float, &edge);
    add("then", Type::Float, &thenValue);
    add("else", Type::Float, &elseValue);
}
//--------------------------------------------------------------
WaveformData::WaveformData() : BaseAttributes(TypeId)
{
    addOutput("value", IoType::Signal);
    add("period", Type::Float, &period);
    add("min", Type::Float, &minVal);
    add("max", Type::Float, &maxVal);
    add("type", Type::Int, &type);
    add("noise-seed", Type::Int, &noiseSeed);
    add("noise-samples", Type::Int, &noiseSamples);
}
//--------------------------------------------------------------
void WaveformData::computeNoise()
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
//--------------------------------------------------------------
void OutputData::ui()
{
    static constexpr size_t sampleRateCount = 6;
    static constexpr size_t sampleFormatCount = 2;
    
    static constexpr std::array<int,sampleRateCount> indexedRates = {AudioSettings::SampleRate_8kHz, AudioSettings::SampleRate_11kHz, AudioSettings::SampleRate_22kHz, AudioSettings::SampleRate_32kHz, AudioSettings::SampleRate_44kHz, AudioSettings::SampleRate_48kHz};
    static constexpr std::array<const char*,sampleRateCount> sampleRateNames = {"SampleRate_8kHz", "SampleRate_11kHz", "SampleRate_22kHz", "SampleRate_32kHz", "SampleRate_44kHz", "SampleRate_48kHz"};
    static constexpr std::array<int,sampleFormatCount> indexedFormats = {AudioSettings::Format_Mono8, AudioSettings::Format_Mono16};
    static constexpr std::array<const char*,sampleFormatCount> sampleFormatNames = {"Format_Mono8", "Format_Mono16"};


    if (ImGui::InputFloat("range", &range)) onChanged();
    if (ImGui::InputFloat("duration scale", &duration)) onChanged();

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
        onChanged();
    }
    
    if (ImGui::Button(sampleFormatNames[sampleFormatIndex]))
    {
        sampleFormatIndex = (sampleFormatIndex+1) % (int)sampleFormatCount;
        sampleBits = indexedFormats[sampleFormatIndex];
        onChanged();
    }

    if (ImGui::Button("Play"))
    {
        toPlay = true;
        onChanged();
    }

    if (ImGui::Button("..."))
        waveExportIo.open();
    ImGui::SameLine();
    ImGui::Text(path.c_str());

    waveExportIo.display();
    if (waveExportIo.confirmed)
        path = waveExportIo.filepath;
    if (ImGui::Button("Export"))
    {
        toExport = true;
        onChanged();
    }
}
//--------------------------------------------------------------
void PolynomialSamplerData::ui()
{
    ImGui::Text("Preview:");
    //preview.compute(this);
    //ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    ImGui::Separator();

    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        coefs.resize(count);
        onChanged();
    }

    if (ImGui::InputFloat("reset", &reset)) onChanged();
    
    ImGui::Text("Coefs");
    ImGui::Separator();
    int index = 0;
    for(auto& coef : coefs)
    {
        std::string textid = std::string("##") + std::to_string(index);
        if (ImGui::InputFloat(textid.c_str(), &coef)) onChanged();
        index++;
    }
}
//--------------------------------------------------------------
void FreqFilterData::ui()
{
    if (ImGui::InputFloat("offset", &offset)) onChanged();
    if (ImGui::InputFloat("length", &length)) onChanged();
    if (ImGui::InputFloat("minGain", &minGain)) onChanged();
    if (ImGui::InputFloat("maxGain", &maxGain)) onChanged();
    if (ImGui::InputFloat("factor", &factor)) onChanged();
    
    ImGui::Separator();
    ImGui::Text("Preview");
    // startSamplingGraph();
    /*std::array<float, 100> buf;
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
    ImGui::PlotLines("##preview", buf.data(), 100, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));*/
}
//--------------------------------------------------------------
void HarmonicsData::ui()
{
    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        freqs.resize(count);
        onChanged();
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
        if (ImGui::InputFloat(keytext.c_str(), &kv.first)) onChanged();
        ImGui::NextColumn();
        if (ImGui::InputFloat(valtext.c_str(), &kv.second)) onChanged();
        ImGui::NextColumn();
        index++;
    }
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::Text("Preview");
    //  preview.compute(this);
    // ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}
//--------------------------------------------------------------
void PitchData::ui()
{
    auto generator = [](float f){
        float duration = 0.3f;
        auto& settings = AudioSettings::defaultSettings;
        qb::Pcm16 output;
        output.resize((size_t)(duration * settings.sampleRate));
        for(unsigned i=0;i<output.count();++i)
        {
            float t = (float)i / (float)settings.sampleRate;
            output.set(i, std::sin(f * t * 2.f*3.141592f));
        }
        return output;
    };

    if (ImGui::InputInt("octave", &octave)) onChanged();
    if (ImGui::InputInt("semitone", &semitone)) onChanged();

    constexpr std::array<const char*, 12> pitch_name = {"C", "C#/Db", "D", "Eb/D#", "E", "F", "F#/Gb", "G", "Ab/G#", "A", "Bb/A#", "B"};
    ImGui::Separator();
    ImGui::Text("info:");
    int midi = getMidiIndex();
    const char* noteName = pitch_name[midi % 12];
    float freq = getFreq();
    ImGui::Text(noteName);
    ImGui::InputInt("midi", &midi);
    ImGui::InputFloat("freq", &freq);
    ImGui::Separator();
    ImGui::Text("Preview");
    // preview.compute(this);
    // ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}
//--------------------------------------------------------------
void WaveformData::ui()
{
    ImGui::Text("Preview:");
    // preview.compute(this);
    // ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
    ImGui::Separator();

    if (ImGui::InputFloat("period", &period)) onChanged();
    if (ImGui::InputFloat("min", &minVal)) onChanged();
    if (ImGui::InputFloat("max", &maxVal)) onChanged();

    constexpr size_t count = 4;
    constexpr std::array<const char*, count> typeNames = {"sin", "saw", "square", "noise"};
    if (ImGui::Button(typeNames[type]))
    {
        type = (type+1) % count;
        onChanged();
    }

    if (type == 3)
    {
        if (ImGui::InputInt("noise seed", &noiseSeed)) onChanged();
        if (ImGui::InputInt("noise size", &noiseSamples)) onChanged();
    }
}
//--------------------------------------------------------------
void KeySamplerData::ui()
{
    constexpr size_t typeCount = 3;
    constexpr std::array<const char*, typeCount> typeNames = {"flat", "linear", "cubic"};
    if (ImGui::Button(typeNames[interpo]))
    {
        interpo = (interpo+1) % typeCount;
        onChanged();
    }

    if (ImGui::InputInt("count", &count))
    {
        if (count < 1) count = 1;
        if (count > 10) count = 10;
        keys.resize(count);
        onChanged();
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
        if (ImGui::InputFloat(keytext.c_str(), &kv.first)) onChanged();
        ImGui::NextColumn();
        if (ImGui::InputFloat(valtext.c_str(), &kv.second)) onChanged();
        ImGui::NextColumn();
        index++;
    }
    ImGui::Columns(1);
    // ImGui::Separator();
    // ImGui::Text("Preview");
    // preview.compute(this);
    // ImGui::PlotLines("##preview", preview.data.data(), 32, 0, NULL, FLT_MAX, FLT_MAX, ImVec2(0, 60.0f));
}
