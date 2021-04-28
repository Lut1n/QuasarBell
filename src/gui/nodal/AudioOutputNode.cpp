#include "gui/nodal/AudioOutputNode.hpp"
#include "signal/operations/OperationType.hpp"

#include "signal/Signal.hpp"

#include "imgui.h"

#include "app/App.hpp"

#include <string>

#include "core/Factory.h"

static TypedFactory<SignalOperationNode, AudioOutputNode> audiooutput_node_factory(qb::OperationType_AudioOutput);

AudioOutputNode* AudioOutputNode::defaultOutput = nullptr;

//--------------------------------------------------------------
AudioOutputNode::AudioOutputNode()
    : SignalOperationNode("Output", qb::OperationType_AudioOutput)
{
    addPin(0, "in 1", false);
    setOperation(&output);

    if (defaultOutput == nullptr) defaultOutput = this;
}

//--------------------------------------------------------------
AudioOutputNode::~AudioOutputNode()
{
    if (defaultOutput == this) defaultOutput = nullptr;
}

//--------------------------------------------------------------
void AudioOutputNode::displayProperties()
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

    getOperation()->validateGraph();
    s_imgui_sampler_set_count(100);
    ImGui::PlotLines("##preview", s_imgui_sampler, getOperation(), 100, 0, NULL, -output.range, output.range, ImVec2(0, 60.0f));
    std::string s = std::to_string(value);
    ImGui::Text(s.c_str());

    if (ImGui::InputFloat("range", &output.range)) dirtyPreview();
    ImGui::InputFloat("duration scale", &output.duration);

    if (sampleRateIndex == -1)
    {
        if(output.sampleRate == 0) output.sampleRate = AudioSettings::SampleRate_44kHz;
        sampleRateIndex = 4;
        for(size_t i=0; i<sampleRateCount; ++i)
        {
            if (output.sampleRate == indexedRates[i])
            {
                sampleRateIndex = i;
                break;
            }
        }
    }
    
    if (sampleFormatIndex == -1)
    {
        if(output.sampleBits == 0) output.sampleBits = AudioSettings::Format_Mono16;
        sampleFormatIndex = 1;
        for(size_t i=0; i<sampleFormatCount; ++i)
        {
            if (output.sampleBits == indexedFormats[i])
            {
                sampleFormatIndex = i;
                break;
            }
        }
    }

    if (ImGui::Button(sampleRateNames[sampleRateIndex]))
    {
        sampleRateIndex = (sampleRateIndex+1) % sampleRateCount;
        output.sampleRate = indexedRates[sampleRateIndex];
    }
    
    if (ImGui::Button(sampleFormatNames[sampleFormatIndex]))
    {
        sampleFormatIndex = (sampleFormatIndex+1) % sampleFormatCount;
        output.sampleBits = indexedFormats[sampleFormatIndex];
    }

    if (ImGui::Button("Play") && App::s_instance)
    {
        auto& sound = *(App::s_instance->sound);
        if(sound.getState() != SoundNode::Playing)
        {
            std::unique_ptr<PcmDataBase> pcm;
            if (output.sampleBits == AudioSettings::Format_Mono8 || output.sampleBits == AudioSettings::Format_Stereo8)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
            if (output.sampleBits == AudioSettings::Format_Mono16 || output.sampleBits == AudioSettings::Format_Stereo16)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
            AudioOutputNode::defaultOutput->generate(*pcm);
            // generate(pcm);
            sound.queue(*pcm.get());
            sound.play();
        }
    }
}

//--------------------------------------------------------------
void AudioOutputNode::generate(PcmDataBase& pcm)
{
    if (pcm.format != output.sampleBits)
        std::cout << "warning: AudioOutputNode::generate() pcm format does not match" << std::endl;
    float duration = output.duration;
    if (duration == 0.0f)
    {
        std::cout << "error: duration = 0; 1s is used" << std::endl;
        duration = 1.0;
    }

    // auto& settings = AudioSettings::defaultSettings;
    auto* operations = getOperation();

    operations->validateGraph();

    /*if (output.sampleBits == AudioSettings::Format_Mono8 || output.sampleBits == AudioSettings::Format_Stereo8)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
    if (output.sampleBits == AudioSettings::Format_Mono16 || output.sampleBits == AudioSettings::Format_Stereo16)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();*/

    pcm.sampleRate = (AudioSettings::SampleRate)output.sampleRate;
    pcm.resize(duration * output.sampleRate);
    
    float sample_t = 1.0 / output.sampleRate;
    
    SignalOperation::Time time;
    time.duration = duration;
    time.elapsed = sample_t;
    for(unsigned i=0;i<pcm.count();++i)
    {
        time.sec = (float)i / (float)output.sampleRate;
        time.t = time.sec / time.duration;
        pcm.set(i, operations->sample(0, time).fvec[0]);
    }
}