#ifndef QUASAR_BELL_POSTFX_HPP
#define QUASAR_BELL_POSTFX_HPP

#include "gui/Gui.hpp"
#include "gui/Keyboard.hpp"
#include "gui/SignalEdit.hpp"

//--------------------------------------------------------------
class RingModFxEdit : public GuiModule
{
public:
    RingModFxEdit();
    
    float sample(float t);
    void resetValues();

    void draw();
    void render() override;

public:
    LinearSampler freqSampler;
    LinearSampler amplSampler;
    WaveEdit wave;
};

//--------------------------------------------------------------
class TremoloFxEdit : public GuiModule
{
public:
    TremoloFxEdit();
    
    float sample(float t);
    void resetValues();
    
    void draw();
    void render() override;

public:
    LinearSampler freqSampler;
    LinearSampler amplSampler;
    
    WaveEdit wave;
    SignalEdit freqPreview;
    SignalEdit amplPreview;
};

//--------------------------------------------------------------
class PostFxPanel : public GuiModule
{
public:
    void render() override;

    RingModFxEdit* ringmod = nullptr;
    TremoloFxEdit* tremolo = nullptr;
};


#endif // QUASAR_BELL_POSTFX_HPP
