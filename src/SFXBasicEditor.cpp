#include "SFXBasicEditor.hpp"

#include "io/FileIO.hpp"
#include "io/SignalRW.hpp"
#include "io/InputRW.hpp"
#include "io/PostRW.hpp"
#include "signal/WavExporter.hpp"

//--------------------------------------------------------------
SFXEditorComponentGroup::SFXEditorComponentGroup()
{
    mainEdit = new MainEdit();
    amplitudeEdit = &mainEdit->amplEdit;
    frequencyEdit = &mainEdit->freqEdit;
    waveEdit = &mainEdit->waveEdit;
    pcmPlayer = new PcmPlayer();
    vibrato = new VibratoFxEdit();
    arpeggio = new ArpeggioFxEdit();
    tremolo = new TremoloFxEdit();
    ringmod = new RingModFxEdit();

    postfx = new PostFxPanel();
    postfx->ringmod = ringmod;
    postfx->tremolo = tremolo;

    inputfx = new InputFxPanel();
    inputfx->vibrato = vibrato;
    inputfx->arpeggio = arpeggio;

    add(mainEdit);
    add(pcmPlayer);
    add(vibrato);
    add(arpeggio);
    add(tremolo);
    add(ringmod);
    add(postfx);
    add(inputfx);
}
//--------------------------------------------------------------
SFXEditorComponentGroup::~SFXEditorComponentGroup()
{
    delete mainEdit;
    delete pcmPlayer;
    delete vibrato;
    delete arpeggio;
    delete tremolo;
    delete ringmod;
    delete postfx;
    delete inputfx;
}

//--------------------------------------------------------------
void BasicEditorWorkSpace::init(App* app)
{
    _app = app;
}
//--------------------------------------------------------------
void BasicEditorWorkSpace::onEvent(const KeyEvent& event)
{
    _doPlay = event.key == 'a' && event.pressed;
}
//--------------------------------------------------------------
void BasicEditorWorkSpace::update(double t)
{
    float duration = _components.amplitudeEdit->duration + _components.amplitudeEdit->release;
    _components.tremolo->freqSampler.duration = duration;
    _components.ringmod->freqSampler.duration = duration;
    _components.vibrato->freqSampler.duration = duration;
    _components.tremolo->amplSampler.duration = duration;
    _components.ringmod->amplSampler.duration = duration;
    _components.vibrato->amplSampler.duration = duration;
    _components.arpeggio->rateSampler.duration = duration;
    
    if(_components.pcmPlayer->playClicked)
    {
        _doPlay = true;
        _components.pcmPlayer->playClicked = false;
    }

    GuiComponent* inputs[] = { _components.waveEdit, _components.frequencyEdit, _components.amplitudeEdit,
                                _components.vibrato, & _components.vibrato->wave, _components.arpeggio,
                                _components.tremolo, & _components.tremolo->wave, _components.ringmod,
                                & _components.ringmod->wave};
    
    bool changed = false;
    for(auto edit : inputs) if(edit->hasChanged) changed = true;
    for(auto edit : inputs) edit->hasChanged = false;
    if(_components.pcmPlayer->autoPlay && changed) _doPlay = true;
    
    auto& sound = *(_app->sound);
    auto& gui = *(_app->gui);
    gui.makeCurrent(&_components);
    
    if(sound.getState() == SoundNode::Playing)
    {
        _components.pcmPlayer->oft = (t-_startTime);
    }
    else if(_doPlay)
    {
        PcmData pcm = generate();
        sound.queue(pcm);
        sound.play();
        _components.pcmPlayer->pcm = pcm;
        _components.pcmPlayer->oft = 0.0f;
        _startTime = t;
    }

    if(gui.fileInput.confirmed)
    {
        if(gui.fileInput.request == UserFileInput::Load_Prj)
        {
            JsonValue root = loadJsonFile(gui.fileInput.filepath);
            loadFrom(root, *_components.waveEdit, *_components.frequencyEdit, *_components.amplitudeEdit);
            loadFrom(root, *_components.vibrato, *_components.arpeggio);
            loadFrom(root, *_components.ringmod, *_components.tremolo);
        }
        else if(gui.fileInput.request == UserFileInput::Save_Prj)
        {
            JsonValue root;
            writeInfo(root);
            saveInto(root, *_components.waveEdit, *_components.frequencyEdit, *_components.amplitudeEdit);
            saveInto(root, *_components.vibrato, *_components.arpeggio);
            saveInto(root, *_components.ringmod, *_components.tremolo);
            saveJsonFile(gui.fileInput.filepath, root);
        }
        gui.fileInput.request = UserFileInput::Nothing;
        gui.fileInput.confirmed = false;
    }
    if(gui.waveInput.confirmed)
    {
        if(gui.waveInput.request == UserFileInput::Export_Wav)
        {
            WavExporter::exportAsWAV(gui.waveInput.filepath, generate());
        }
        gui.waveInput.request = UserFileInput::Nothing;
        gui.waveInput.confirmed = false;
    }
    
    _doPlay = false;
}
//--------------------------------------------------------------
void BasicEditorWorkSpace::render()
{
}
//--------------------------------------------------------------
qb::Pcm16 BasicEditorWorkSpace::generate()
{
    auto& ampl = *_components.amplitudeEdit;
    auto& freq = *_components.frequencyEdit;
    auto& wave = *_components.waveEdit;
    auto& vibrato = *_components.vibrato;
    auto& arpeggio = *_components.arpeggio;
    auto& tremolo = *_components.tremolo;
    auto& ringmod = *_components.ringmod;

    auto& settings = AudioSettings::defaultSettings;
    float duration = ampl.duration + ampl.release;
    
    qb::Pcm16 output;
    output.resize(duration * settings.sampleRate);
    
    float phase = 0.0f;
    float sample_t = 1.0 / settings.sampleRate;
    
    for(unsigned i=0;i<output.count();++i)
    {
        float t = (float)i / (float)settings.sampleRate;
        
        float f = freq.sample(t/duration);
        float a = ampl.sample(t);
        
        if(arpeggio.isEnabled) f = arpeggio.sample(t, f);
        if(vibrato.isEnabled) f += vibrato.sample(t);
        
        phase += f * sample_t;
        float r = wave.sample(phase) * a;
        if(tremolo.isEnabled) r *= tremolo.sample(t);
        if(ringmod.isEnabled) r *= ringmod.sample(t);
        output.set(i, r);
        
    }
    return output;
}