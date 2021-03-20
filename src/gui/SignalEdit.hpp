#ifndef QUASAR_BELL_SIGNAL_EDIT_HPP
#define QUASAR_BELL_SIGNAL_EDIT_HPP

#include <array>

#include "signal/PcmData.hpp"
#include "signal/LinearSamplers.hpp"
#include "gui/Gui.hpp"

//--------------------------------------------------------------
class SignalEdit : public GuiComponent
{
public:
    void render() override;

    static bool draw(LinearSampler& sampler);
    static float imgui_sampler(void* data, int idx);
};

//--------------------------------------------------------------
class FrequencyEdit : public GuiComponent
{
public:
    float sample(float t);
    static float imgui_sampler(void* data, int idx);
    
    void resetValues();
    void draw();
    void render() override;
    
public:
    float pitch = qb::log2(440.0f);
    float speed = 0.0f;
    float acc = 0.0f;
    float jerk = 0.0f;
    float resetTime = 1.0f;
};

//--------------------------------------------------------------
class AmplitudeEdit : public GuiComponent
{
public:
    float sample(float t);
    static float imgui_sampler(void* data, int idx);
    
    void resetValues();
    void draw();
    void render() override;
    
public:
    // ADSR + duration
    float attack = 0.01f;
    float decay = 0.01f;
    float sustain = 0.5f;
    float release = 0.05f;
    float duration = 0.2f;
};

//--------------------------------------------------------------
class WaveEdit : public GuiComponent
{
public:
    enum Wave
    {
        Sin,
        Saw,
        Square,
        Noise_1,
        Noise_2,
        Noise_3,
        Noise_4
    };
    WaveEdit();
    
    float sample(float t);
    
    void draw();
    void render() override;
    
public:
    Wave wave = Sin;
    const char* selectedText = nullptr;
    
private:
    std::array<float, 4> noiseData1;
    std::array<float, 8> noiseData2;
    std::array<float, 16> noiseData3;
    std::array<float, 32> noiseData4;
};

//--------------------------------------------------------------
class MainEdit : public GuiComponent
{
public:
    void draw();
    void render() override;
    
public:
    FrequencyEdit freqEdit;
    AmplitudeEdit amplEdit;
    WaveEdit waveEdit;
};

//--------------------------------------------------------------
class PcmPlayer : public GuiComponent
{
public:
    void draw();
    void render() override;
 
    static float sample(void* data, int t);

public:
    PcmData pcm;

    float oft = 0.0f;
    bool autoPlay = true;
    bool playClicked = false;
};

#endif // QUASAR_BELL_SIGNAL_EDIT_HPP
