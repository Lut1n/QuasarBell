#ifndef QUASAR_BELL_BASIC_EDITOR
#define QUASAR_BELL_BASIC_EDITOR

#include "App.hpp"
#include "gui/PostEffect.hpp"
#include "gui/InputEffect.hpp"

//--------------------------------------------------------------
class SFXEditModules : public ModuleGroup
{
public:
    SFXEditModules();
    virtual ~SFXEditModules();
    
public:
    AmplitudeEdit* amplitudeEdit;
    FrequencyEdit* frequencyEdit;
    WaveEdit* waveEdit;
    MainEdit* mainEdit;
    PcmPlayer* pcmPlayer;

    InputFxPanel* inputfx;
    VibratoFxEdit* vibrato;
    ArpeggioFxEdit* arpeggio;

    PostFxPanel* postfx;
    TremoloFxEdit* tremolo;
    RingModFxEdit* ringmod;
};

//--------------------------------------------------------------
class BasicEditorWorkSpace : public ToolWorkSpace
{
public:
    void init(App* app) override;
    void onEvent(const KeyEvent& event) override;
    void update(double t) override;
    void render() override;

private:
    PcmData generate();

private:
    SFXEditModules _modules;
    App* _app = nullptr;
    float _startTime = 0.0;
    bool _doPlay = false;
};

#endif // QUASAR_BELL_BASIC_EDITOR
