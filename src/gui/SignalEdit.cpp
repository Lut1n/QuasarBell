#include "SignalEdit.hpp"
 
#include "imgui.h"

#include "signal/Signal.hpp"

//--------------------------------------------------------------
float SignalEdit::imgui_sampler(void* data, int idx)
{
    LinearSampler& sampler = *(LinearSampler*)data;
    return sampler((float)idx/100 * sampler.duration);
}
//--------------------------------------------------------------
bool SignalEdit::draw(LinearSampler& sampler)
{
    bool hasChanged = false;
    ImGui::SliderFloat("value", &sampler.value, sampler.minVal, sampler.maxVal); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("speed", &sampler.speed, sampler.minSpeed, sampler.maxSpeed); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("period", &sampler.resetTime, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::PlotLines("##signalview", imgui_sampler, &sampler, 100, 0, NULL, sampler.minVal, sampler.maxVal, ImVec2(0, 60.0f));
    return hasChanged;
}
//--------------------------------------------------------------
void SignalEdit::render()
{
    ImGui::Begin("Signal Edit");
    ImGui::End();
}
//--------------------------------------------------------------
void FrequencyEdit::resetValues()
{
    pitch = qb::log2(440.0f);
    speed = 0.0f;
    acc = 0.0f;
    jerk = 0.0f;
    resetTime = 1.0f;
    hasChanged = true;
}
//--------------------------------------------------------------
float FrequencyEdit::imgui_sampler(void* data, int idx)
{
    FrequencyEdit& sampler = *(FrequencyEdit*)data;
    return qb::log2(sampler.sample((float)idx/100));
}
//--------------------------------------------------------------
void FrequencyEdit::draw()
{
    float min = 7.0f;
    float max = 12.0;
    
    ImGui::PlotLines("##plotFrecency", imgui_sampler, this, 100, 0, NULL, min, max, ImVec2(0, 80.0f));
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("Frequency");
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    
    ImGui::SliderFloat("pitch", &pitch, min, max); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("speed", &speed, -30, 30.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("acc", &acc, -30.0f, 30.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("jerk", &jerk, -30.0f, 30.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("period", &resetTime, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
}
//--------------------------------------------------------------
void FrequencyEdit::render()
{
    ImGui::Begin("Frequency Edit");
    draw();
    ImGui::End();
}
//--------------------------------------------------------------
float FrequencyEdit::sample(float t)
{
    if(resetTime == 0.0f)
    {
        t = 0.0f;
    }
    else
    {
        while(t>=resetTime) t -= resetTime;
    }
    
    float p = pitch + (speed + (acc + jerk * t) * t) * t;
    float f = qb::pow2(p);
    f = f > 20000.0 ? 20000.0 : (f<0.0 ? 0.0 : f);
    return f;
}
//--------------------------------------------------------------
void AmplitudeEdit::resetValues()
{
    attack = 0.01f;
    decay = 0.01f;
    sustain = 0.5f;
    release = 0.05f;
    duration = 0.2f;
    hasChanged = true;
}
//--------------------------------------------------------------
float AmplitudeEdit::imgui_sampler(void* data, int idx)
{
    AmplitudeEdit& sampler = *(AmplitudeEdit*)data;
    return sampler.sample((float)idx/256 * (sampler.duration+sampler.release));
}
//--------------------------------------------------------------
void AmplitudeEdit::draw()
{
    ImGui::PlotLines("##plotAmplitude", imgui_sampler, this, 256, 0, NULL, 0.0, 1.0, ImVec2(0, 80.0f));
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("Amplitude");
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    
    ImGui::SliderFloat("attack", &attack, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("decay", &decay, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("sustain", &sustain, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("release", &release, 0.0f, 1.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
    ImGui::SliderFloat("duration", &duration, attack+decay, 5.0f); if(ImGui::IsItemDeactivated()) hasChanged = true;
}
//--------------------------------------------------------------
void AmplitudeEdit::render()
{
    ImGui::Begin("Amplitude Edit");
    draw();
    ImGui::End();
}
//--------------------------------------------------------------
float AmplitudeEdit::sample(float t)
{
    float rt = 0.0f;
    if(t < attack)
    {
        rt = t / attack;
    }
    else if(t-attack < decay)
    {
        rt = 1.0f - (1.0f - sustain) * (t-attack) / decay;
    }
    else if(t < duration)
    {
        rt = sustain;
    }
    else if(t < duration + release)
    {
        rt = sustain * (1.0f-(t-duration)/release);
    }
    else
    {
        rt = 0.0f;
    }
    rt = rt > 1.0 ? 1.0 : (rt<-0.0 ? 0.0 : rt);
    return rt;
}
//--------------------------------------------------------------
void WaveEdit::draw()
{
    int sample_count = 60;
    float min = -1.0f;
    float max = 1.0f;
    
    int n = sample_count;
    std::vector<float> output; output.resize(n);
    for(int i=0; i<n; ++i)
    {
        float t = (float)i / (float) n;
        output[i] = sample(t);
    }
    
    ImGui::PlotLines("Wave", output.data(), sample_count, 0, NULL, min, max, ImVec2(40, 30.0f));
    
    static const char* items[] = {"Sin", "Saw", "Square", "Noise_1", "Noise_2", "Noise_3", "Noise_4"};
    if(selectedText == nullptr) selectedText = items[0];
    if(ImGui::BeginCombo("##comboWave", selectedText))
    {
        for(int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (selectedText == items[n]);
            if(ImGui::Selectable(items[n], is_selected))
            {
                selectedText = items[n];
                if(n == 0) wave = Sin;
                else if(n == 1) wave = Saw;
                else if(n == 2) wave = Square;
                else if(n == 3) wave = Noise_1;
                else if(n == 4) wave = Noise_2;
                else if(n == 5) wave = Noise_3;
                else if(n == 6) wave = Noise_4;
                hasChanged = true;
            }
            if(is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
//--------------------------------------------------------------
void WaveEdit::render()
{
    ImGui::Begin("Wave Edit");
    draw();
    ImGui::End();
}

//--------------------------------------------------------------
static int s_hold_rand = 0;
//--------------------------------------------------------------
void platform_ind_srand(int seed)
{
    s_hold_rand = seed;
}
//--------------------------------------------------------------
int platform_ind_rand()
{
    return(((s_hold_rand = s_hold_rand * 214013L + 2531011L) >> 16) & 0x7fff);
}

//--------------------------------------------------------------
float noise_value()
{
    const int R_MAX = 100;
    return (float)(platform_ind_rand()%(1+R_MAX)) / R_MAX;
}
//--------------------------------------------------------------
WaveEdit::WaveEdit()
{
    platform_ind_srand(789456123);

    for(unsigned i=0; i<noiseData1.size(); ++i)
        noiseData1[i] = noise_value() * 2.0 - 1.0;
    for(unsigned i=0; i<noiseData2.size(); ++i)
        noiseData2[i] = noise_value() * 2.0 - 1.0;
    for(unsigned i=0; i<noiseData3.size(); ++i)
        noiseData3[i] = noise_value() * 2.0 - 1.0;
    for(unsigned i=0; i<noiseData4.size(); ++i)
        noiseData4[i] = noise_value() * 2.0 - 1.0;
}
//--------------------------------------------------------------
float WaveEdit::sample(float t)
{
    float rt = 0.0f;
    if(wave == Sin)
    {
        rt = std::sin(2.0f*3.141592f * t);
    }
    else if(wave == Saw)
    {
        while(t>1.0) t -= 1.0;
        if(t<0.25f)
        {
            rt = t/0.25f;
        }
        else if(t<0.50f)
        {
            rt = 1.0f - (t-0.25f)/0.25f;
        }
        else if(t<0.75f)
        {
            rt = -(t-0.50f)/0.25f;
        }
        else
        {
            rt = (t-0.75f)/0.25f - 1.0f;
        }
    }
    else if(wave == Square)
    {
        rt = std::sin(2.0f*3.141592f * t) >0.0f ? 1.0 : -1.0;
    }
    else if(wave == Noise_1)
    {
        while(t>1.0) t -= 1.0;
        rt = noiseData1[(unsigned)(t * noiseData1.size())];
    }
    else if(wave == Noise_2)
    {
        while(t>1.0) t -= 1.0;
        rt = noiseData2[(unsigned)(t * noiseData2.size())];
    }
    else if(wave == Noise_3)
    {
        while(t>1.0) t -= 1.0;
        rt = noiseData3[(unsigned)(t * noiseData3.size())];
    }
    else if(wave == Noise_4)
    {
        while(t>1.0) t -= 1.0;
        rt = noiseData4[(unsigned)(t * noiseData4.size())];
    }
    rt = rt > 1.0 ? 1.0 : (rt<-1.0 ? -1.0 : rt);
    return rt;
}
//--------------------------------------------------------------
void MainEdit::draw()
{
    waveEdit.draw();
    ImGui::Separator();
    
    ImGui::Columns(2);
    
    ImGui::BeginGroup();
    ImGui::PushID("freqPanel");
    freqEdit.draw();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::NextColumn();
    
    ImGui::BeginGroup();
    ImGui::PushID("amplPanel");
    amplEdit.draw();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::Columns(1);
}
//--------------------------------------------------------------
void MainEdit::render()
{
    ImGui::SetNextWindowPos(ImVec2(420, 180), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(555, 370), ImGuiCond_FirstUseEver);
    ImGui::Begin("Signal Generator");
    draw();
    ImGui::End();
}
//--------------------------------------------------------------
float PcmPlayer::sample(void* data, int t)
{
    PcmPlayer* that = static_cast<PcmPlayer*>(data);
    float q= std::numeric_limits<short>::max();
    
    float rate = AudioSettings::defaultSettings.sampleRate;
    float displayCount = 10000.0;
    float start_pos  = displayCount - that->oft * rate;
    
    unsigned i = t - start_pos;
    if( i < 0 || i>= that->pcm.samples.size() )
    {
        return 0.0f;
    }
    else
    {
        return that->pcm.samples[i] / q;
    }
}
//--------------------------------------------------------------
void PcmPlayer::draw()
{
    float displayCount = 10000.0;
    float min = -1.0;
    float max = 1.0;
    ImGui::PlotHistogram("Stream", &PcmPlayer::sample, this, displayCount, 0, NULL, min, max, ImVec2(0, 80.0f));
    if( ImGui::Button("Play") )
        playClicked = true;
    ImGui::SameLine();
    ImGui::Checkbox("Auto", &autoPlay);
}
//--------------------------------------------------------------
void PcmPlayer::render()
{
    ImGui::SetNextWindowPos(ImVec2(420, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(555, 150), ImGuiCond_FirstUseEver);
    ImGui::Begin("PCM Player");
    draw();
    ImGui::End();
}
