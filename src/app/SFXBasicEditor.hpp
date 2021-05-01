#ifndef QUASAR_BELL_BASIC_EDITOR
#define QUASAR_BELL_BASIC_EDITOR

#include "App.hpp"
#include "gui/PostEffect.hpp"
#include "gui/InputEffect.hpp"

//--------------------------------------------------------------
class SFXEditorComponentGroup : public GuiComponentGroup
{
public:
    SFXEditorComponentGroup();
    virtual ~SFXEditorComponentGroup();
    
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
    void update(float t) override;
    void render() override;

private:
    qb::Pcm16 generate();

private:
    SFXEditorComponentGroup _components;
    App* _app = nullptr;
    float _startTime = 0.0;
    bool _doPlay = false;
};

#endif // QUASAR_BELL_BASIC_EDITOR
