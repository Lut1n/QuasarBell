#include "Synth.hpp"

#include "signal/Signal.hpp"
#include "signal/Modulator.hpp"

//--------------------------------------------------------------
SynthModules::SynthModules()
{
    keyboard = new Keyboard();
    add(keyboard);
}
//--------------------------------------------------------------
SynthModules::~SynthModules()
{
    delete keyboard;
}
//--------------------------------------------------------------
void SynthWorkSpace::init(App* app)
{
    _app = app;
    _keyMap['a'] = 60;
    _keyMap['s'] = 62;
    _keyMap['d'] = 64;
    _keyMap['f'] = 65;
    _keyMap['g'] = 67;
    _keyMap['h'] = 69;
    _keyMap['j'] = 71;
    _keyMap['k'] = 72;
    
    _keyMap['w'] = 61;
    _keyMap['e'] = 63;
    
    _keyMap['t'] = 66;
    _keyMap['y'] = 68;
    _keyMap['u'] = 70;
    
    for(auto& ks : _keyStates)
    {
        ks = KeyState::None;
    }
}
//--------------------------------------------------------------
void SynthWorkSpace::_onEventImpl(int keyId, bool pressed)
{
    int keyIndex = keyId - _firstKeyID;
    if(pressed)
    {
        if(_keyStates[keyIndex] == KeyState::None)
            _keyStates[keyIndex] = KeyState::Pressed;
        else if(_keyStates[keyIndex] == KeyState::Pressed)
            _keyStates[keyIndex] = KeyState::Maintained;
        /*else if(_keyStates[keyIndex] == KeyState::Maintained)
            _keyStates[keyIndex] = KeyState::Maintained;*/
        else if(_keyStates[keyIndex] == KeyState::Released)
            _keyStates[keyIndex] = KeyState::Pressed;
    }
    else
    {
        /*if(_keyStates[keyIndex] == KeyState::None)
            _keyStates[keyIndex] = KeyState::None;
        else*/ if(_keyStates[keyIndex] == KeyState::Pressed)
            _keyStates[keyIndex] = KeyState::Released;
        else if(_keyStates[keyIndex] == KeyState::Maintained)
            _keyStates[keyIndex] = KeyState::Released;
        else if(_keyStates[keyIndex] == KeyState::Released)
            _keyStates[keyIndex] = KeyState::None;
    }
}
//--------------------------------------------------------------
void SynthWorkSpace::onEvent(const KeyEvent& event)
{
    if(_keyMap.count(event.key) == 1)
    {
        int keyId = _keyMap[event.key];
        _onEventImpl(keyId, event.pressed);
    }
}
//--------------------------------------------------------------
void SynthWorkSpace::update(double t)
{
    for(auto& e : _modules.keyboard->events)
    {
        bool pressed = e.state == Keyboard::KeyState::Pressed;
        _onEventImpl(e.id, pressed);
    }
    _modules.keyboard->events.clear();
    
    auto& sound = *(_app->sound);
    auto& gui = *(_app->gui);
    
    gui.setModuleGroup(&_modules);
    _modules.keyboard->reset();

    float fracTime = 0.02;
    
    for(unsigned keyIndex = 0; keyIndex < _keyStates.size(); ++keyIndex)
    {
        int keyID = keyIndex + _firstKeyID;
        
        if(_keyStates[keyIndex] == KeyState::Pressed)
        {
            _modules.keyboard->setPressed(keyID);
            PcmData pcm = Modulator::attack(pitchToFreq(keyID), fracTime, 1.0f, 0.5f, _genTime);
            _toQueue.push_back(pcm);
            _keyStates[keyIndex] = KeyState::Maintained;
        }
        else if(_keyStates[keyIndex] == KeyState::Maintained)
        {
            _modules.keyboard->setPressed(keyID);
            PcmData pcm = Modulator::generate(pitchToFreq(keyID), fracTime, 0.5f, 0.0f, _genTime);
            _toQueue.push_back(pcm);
        }
        else if(_keyStates[keyIndex] == KeyState::Released)
        {
            PcmData pcm = Modulator::release(pitchToFreq(keyID), fracTime, 0.5f, _genTime);
            _toQueue.push_back(pcm);
            _keyStates[keyIndex] = KeyState::None;
        }
    }
    
    if(sound._PcmDatas.size() < 1)
    {
        PcmData pcm;
        if(_toQueue.size() == 0)
            pcm = Modulator::silent(fracTime);
        else
            pcm = Modulator::mix(_toQueue, fracTime);
        _toQueue.clear();

        sound.queue(pcm);
        _genTime += fracTime;
    }
    
    if(sound.getState() != SoundNode::Playing)
    {
        sound.play();
    }
}
//--------------------------------------------------------------
void SynthWorkSpace::render()
{
}
