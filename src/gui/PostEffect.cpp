#include "PostEffect.hpp"

#include "imgui.h"

#include "signal/Signal.hpp"

//--------------------------------------------------------------
RingModFxEdit::RingModFxEdit()
{
    resetValues();
}
//--------------------------------------------------------------
void RingModFxEdit::resetValues()
{
    freqSampler.value = 1000.0;
    freqSampler.speed = 0.0;
    freqSampler.resetTime = 1.0;
    freqSampler.minVal = 30.0;
    freqSampler.maxVal = 6000.0;
    freqSampler.minSpeed = -1000.0;
    freqSampler.maxSpeed = 1000.0;
    
    amplSampler.value = 0.5;
    amplSampler.speed = 0.0f;
    amplSampler.resetTime = 1.0f;
    amplSampler.minVal = 0.0f;
    amplSampler.maxVal = 1.0f;
    amplSampler.minSpeed = -1.0;
    amplSampler.maxSpeed = 1.0;
    
    if(isEnabled) hasChanged = true;
}
//--------------------------------------------------------------
void RingModFxEdit::draw()
{
    ImGui::PushID("ringmod-fx");
    ImGui::Text("Ring Mod. Fx Edit");
    ImGui::BeginGroup();
    if(ImGui::Checkbox("enable", &isEnabled)) hasChanged = true;
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    ImGui::SameLine();
    wave.draw();
    
    ImGui::Columns(2);
    
    ImGui::BeginGroup();
    ImGui::PushID("ringmodFreq");
    ImGui::Text("frequency");
    if(SignalEdit::draw(freqSampler) && isEnabled) hasChanged = true;
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::NextColumn();
    
    ImGui::BeginGroup();
    ImGui::PushID("ringmodAmpl");
    ImGui::Text("amplitude");
    if(SignalEdit::draw(amplSampler) && isEnabled) hasChanged = true;
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::Columns(1);
    ImGui::PopID();
}
//--------------------------------------------------------------
void RingModFxEdit::render()
{
}
//--------------------------------------------------------------
float RingModFxEdit::sample(float t)
{  
    float f = freqSampler(t);
    float a = amplSampler(t);
    
    float signal = wave.sample(t * f);
    float rt = (1.f-a) + (signal * 0.5f + 0.5f) * a;
    return rt;
}
//--------------------------------------------------------------
TremoloFxEdit::TremoloFxEdit()
{
    resetValues();
}
//--------------------------------------------------------------
void TremoloFxEdit::draw()
{
    ImGui::PushID("tremolo-fx");
    ImGui::Text("Tremolo Fx Edit");
    ImGui::BeginGroup();
    if(ImGui::Checkbox("enable", &isEnabled)) hasChanged = true;
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    ImGui::SameLine();
    wave.draw();
    
    ImGui::Columns(2);
    
    ImGui::BeginGroup();
    ImGui::PushID("tremoloFreq");
    ImGui::Text("frequency");
    if( SignalEdit::draw(freqSampler) && isEnabled) hasChanged = true;
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::NextColumn();
    
    ImGui::BeginGroup();
    ImGui::PushID("tremoloAmpl");
    ImGui::Text("amplitude");
    if (SignalEdit::draw(amplSampler) && isEnabled) hasChanged = true;
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::Columns(1);
    ImGui::PopID();
}
//--------------------------------------------------------------
void TremoloFxEdit::render()
{
}
//--------------------------------------------------------------
void TremoloFxEdit::resetValues()
{
    freqSampler.value = 10.0;
    freqSampler.speed = 0.0;
    freqSampler.resetTime = 1.0;
    freqSampler.minVal = 0.0;
    freqSampler.maxVal = 50.0;
    freqSampler.minSpeed = -10.0;
    freqSampler.maxSpeed = 10.0;
    
    amplSampler.value = 0.5;
    amplSampler.speed = 0.0f;
    amplSampler.resetTime = 1.0f;
    amplSampler.minVal = 0.0f;
    amplSampler.maxVal = 1.0f;
    amplSampler.minSpeed = -1.0;
    amplSampler.maxSpeed = 1.0;
    
    if(isEnabled) hasChanged = true;
}
//--------------------------------------------------------------
float TremoloFxEdit::sample(float t)
{
    float f = freqSampler(t);
    float a = amplSampler(t);
    
    float signal = wave.sample(t * f);
    float rt = (1.f-a) + (signal * 0.5f + 0.5f) * a;
    return rt;
}
//--------------------------------------------------------------
void PostFxPanel::render()
{
    ImGui::SetNextWindowPos(ImVec2(980, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(410, 530), ImGuiCond_FirstUseEver);
    ImGui::Begin("Post-Fx");
    tremolo->draw();
    ImGui::Separator();
    ringmod->draw();
    ImGui::End();
}