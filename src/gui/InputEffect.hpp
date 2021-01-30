#ifndef QUASAR_BELL_INPUT_FX_HPP
#define QUASAR_BELL_INPUT_FX_HPP

#include "gui/Gui.hpp"
#include "gui/Keyboard.hpp"
#include "gui/SignalEdit.hpp"

//--------------------------------------------------------------
class VibratoFxEdit : public GuiModule
{
public:
    VibratoFxEdit();
    
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
class ArpeggioFxEdit : public GuiModule
{
public:
    ArpeggioFxEdit();
    
    float sample(float t, float noteRef);
    void resetValues();

    void draw();
    void render() override;

public:
    std::vector<int> notes = {69};

    Keyboard* miniKeyboard;
    LinearSampler rateSampler;
    SignalEdit ratePreview;
};

//--------------------------------------------------------------
class InputFxPanel : public GuiModule
{
public:
    void render() override;

    VibratoFxEdit* vibrato = nullptr;
    ArpeggioFxEdit* arpeggio = nullptr;
};


#endif // QUASAR_BELL_INPUT_FX_HPP
