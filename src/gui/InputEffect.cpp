#include "InputEffect.hpp"

#include <algorithm>

#include "imgui.h"

#include "signal/Signal.hpp"


//--------------------------------------------------------------
VibratoFxEdit::VibratoFxEdit()
{
    resetValues();
}
//--------------------------------------------------------------
void VibratoFxEdit::resetValues()
{
    freqSampler.value = 10.0;
    freqSampler.speed = 0.0;
    freqSampler.resetTime = 1.0;
    freqSampler.minVal = 0.0;
    freqSampler.maxVal = 30.0;
    freqSampler.minSpeed = -10.0;
    freqSampler.maxSpeed = 10.0;
    
    amplSampler.value = 20.0;
    amplSampler.speed = 0.0f;
    amplSampler.resetTime = 1.0f;
    amplSampler.minVal = 0.0f;
    amplSampler.maxVal = 50.0f;
    amplSampler.minSpeed = -30.0;
    amplSampler.maxSpeed = 30.0;
    
    if(isEnabled) hasChanged = true;
}
//--------------------------------------------------------------
void VibratoFxEdit::draw()
{
    ImGui::PushID("vibrato-fx");
    ImGui::Text("VibratoFx Edit");
    ImGui::BeginGroup();
    if(ImGui::Checkbox("enable", &isEnabled)) hasChanged = true;
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    ImGui::SameLine();
    wave.draw();
    
    ImGui::Columns(2);
    
    ImGui::BeginGroup();
    ImGui::PushID("vibratoFreq");
    ImGui::Text("frequency");
    if(SignalEdit::draw(freqSampler) && isEnabled) hasChanged = true;
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::NextColumn();
    
    ImGui::BeginGroup();
    ImGui::PushID("vibratoAmpl");
    ImGui::Text("amplitude");
    if(SignalEdit::draw(amplSampler) && isEnabled) hasChanged = true;
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::Columns(1);
    
    ImGui::PopID();
}
//--------------------------------------------------------------
void VibratoFxEdit::render()
{

}
//--------------------------------------------------------------
float VibratoFxEdit::sample(float t)
{
    float f = freqSampler(t);
    float a = amplSampler(t);
    
    float signal = wave.sample(t * f);
    float rt = signal * a;
    return rt;
}
//--------------------------------------------------------------
ArpeggioFxEdit::ArpeggioFxEdit()
{
    miniKeyboard = new Keyboard(60, 71);
    resetValues();
}
//--------------------------------------------------------------
void ArpeggioFxEdit::resetValues()
{
    notes = {69};
    
    rateSampler.value = 6.0;
    rateSampler.speed = 0.0;
    rateSampler.resetTime = 1.0;
    rateSampler.minVal = 0.0;
    rateSampler.maxVal = 10.0;
    rateSampler.minSpeed = -10.0;
    rateSampler.maxSpeed = 10.0;
    
    if(isEnabled) hasChanged = true;
}
//--------------------------------------------------------------
void ArpeggioFxEdit::draw()
{
    ImGui::PushID("arpeggio-fx");
    for(auto& e : miniKeyboard->events)
    {
        auto it = std::find(notes.begin(), notes.end(), e.id);
        if(e.state == Keyboard::KeyState::Pressed)
        {
            if(it == notes.end())
            {
                notes.push_back(e.id);
            }
            else
            {
                notes.erase(it);
            }
            if(isEnabled) hasChanged = true;
        }
    }
    miniKeyboard->events.clear();
    
    miniKeyboard->reset();
    for(auto& n : notes)
    {
        miniKeyboard->setPressed(n);
    }
    
    ImGui::Text("ArpeggioFx Edit");
    ImGui::BeginGroup();
    if(ImGui::Checkbox("enable", &isEnabled)) hasChanged = true;
    if(ImGui::Button("Reset")) resetValues();
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginChild("notes", ImVec2(100, 60), true);
    miniKeyboard->draw();
    ImGui::EndChild();
    
    ImGui::BeginGroup();
    ImGui::PushID("arpeggioRate");
    ImGui::Text("rate");
    if(SignalEdit::draw(rateSampler) && isEnabled) hasChanged = true;
    ImGui::SameLine();
    ImGui::PopID();
    ImGui::EndGroup();
    
    ImGui::PopID();
}
//--------------------------------------------------------------
void ArpeggioFxEdit::render()
{
}
//--------------------------------------------------------------
float ArpeggioFxEdit::sample(float t, float noteRef)
{
    if(notes.size() == 0)
        return noteRef;
    
    float ef_rate = rateSampler(t);
    
    int n = (int)std::floor(t * ef_rate);
    
        n = n % notes.size();
    if(n < notes.size())
    {
        return noteRef + pitchToFreq((float)notes[n]) - 440.f;
    }
    return 0.0;
}
//--------------------------------------------------------------
void InputFxPanel::render()
{
    ImGui::SetNextWindowPos(ImVec2(5, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(410, 530), ImGuiCond_FirstUseEver);
    ImGui::Begin("Input-Fx");
    vibrato->draw();
    ImGui::Separator();
    arpeggio->draw();
    ImGui::End();
}