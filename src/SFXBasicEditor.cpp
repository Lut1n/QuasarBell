#include "SFXBasicEditor.hpp"

#include "io/FileIO.hpp"
#include "io/SignalRW.hpp"
#include "io/InputRW.hpp"
#include "io/PostRW.hpp"
#include "signal/WavExporter.hpp"

//--------------------------------------------------------------
SFXEditModules::SFXEditModules()
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
SFXEditModules::~SFXEditModules()
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
    float duration = _modules.amplitudeEdit->duration + _modules.amplitudeEdit->release;
    _modules.tremolo->freqSampler.duration = duration;
    _modules.ringmod->freqSampler.duration = duration;
    _modules.vibrato->freqSampler.duration = duration;
    _modules.tremolo->amplSampler.duration = duration;
    _modules.ringmod->amplSampler.duration = duration;
    _modules.vibrato->amplSampler.duration = duration;
    _modules.arpeggio->rateSampler.duration = duration;
    
    if(_modules.pcmPlayer->playClicked)
    {
        _doPlay = true;
        _modules.pcmPlayer->playClicked = false;
    }

    GuiModule* editModules[] = {_modules.waveEdit, _modules.frequencyEdit, _modules.amplitudeEdit,
                                _modules.vibrato, &_modules.vibrato->wave, _modules.arpeggio,
                                _modules.tremolo, &_modules.tremolo->wave, _modules.ringmod,
                                &_modules.ringmod->wave};
    
    bool changed = false;
    for(auto edit : editModules) if(edit->hasChanged) changed = true;
    for(auto edit : editModules) edit->hasChanged = false;
    if(_modules.pcmPlayer->autoPlay && changed) _doPlay = true;
    
    auto& sound = *(_app->sound);
    auto& gui = *(_app->gui);
    gui.setModuleGroup(&_modules);
    
    if(sound.getState() == SoundNode::Playing)
    {
        _modules.pcmPlayer->oft = (t-_startTime);
    }
    else if(_doPlay)
    {
        PcmData pcm = generate();
        sound.queue(pcm);
        sound.play();
        _modules.pcmPlayer->pcm = pcm;
        _modules.pcmPlayer->oft = 0.0f;
        _startTime = t;
    }

    if(gui.fileInput.confirmed)
    {
        if(gui.fileInput.request == UserFileInput::Load_Prj)
        {
            JsonValue root = loadJsonFile(gui.fileInput.filepath);
            loadFrom(root, *_modules.waveEdit, *_modules.frequencyEdit, *_modules.amplitudeEdit);
            loadFrom(root, *_modules.vibrato, *_modules.arpeggio);
            loadFrom(root, *_modules.ringmod, *_modules.tremolo);
        }
        else if(gui.fileInput.request == UserFileInput::Save_Prj)
        {
            JsonValue root;
            writeInfo(root);
            saveInto(root, *_modules.waveEdit, *_modules.frequencyEdit, *_modules.amplitudeEdit);
            saveInto(root, *_modules.vibrato, *_modules.arpeggio);
            saveInto(root, *_modules.ringmod, *_modules.tremolo);
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
PcmData BasicEditorWorkSpace::generate()
{
    auto& ampl = *_modules.amplitudeEdit;
    auto& freq = *_modules.frequencyEdit;
    auto& wave = *_modules.waveEdit;
    auto& vibrato = *_modules.vibrato;
    auto& arpeggio = *_modules.arpeggio;
    auto& tremolo = *_modules.tremolo;
    auto& ringmod = *_modules.ringmod;

    auto& settings = AudioSettings::defaultSettings;
    float duration = ampl.duration + ampl.release;
    
    PcmData output;
    output.samples.resize(duration * settings.sampleRate);
    
    short quantizer = std::numeric_limits<short>::max();
    
    float phase = 0.0f;
    float sample_t = 1.0 / settings.sampleRate;
    
    for(unsigned i=0;i<output.samples.size();++i)
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
        output.samples[i] = r * quantizer;
        
    }
    return output;
}