#include "Synth.hpp"

#include "signal/Signal.hpp"
#include "signal/Modulator.hpp"

//--------------------------------------------------------------
SynthComponentGroup::SynthComponentGroup()
{
    keyboard = new Keyboard();
    add(keyboard);
}
//--------------------------------------------------------------
SynthComponentGroup::~SynthComponentGroup()
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
    int keyIndex = keyId - (int)_firstKeyID;
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
void SynthWorkSpace::update(float t)
{
    for(auto& e : _components.keyboard->events)
    {
        bool pressed = e.state == Keyboard::KeyState::Pressed;
        _onEventImpl(e.id, pressed);
    }
    _components.keyboard->events.clear();
    
    auto& sound = *(_app->sound);
    auto& gui = *(_app->gui);
    
    gui.makeCurrent(&_components);
    _components.keyboard->reset();

    float fracTime = 0.02f;
    
    for(unsigned keyIndex = 0; keyIndex < _keyStates.size(); ++keyIndex)
    {
        int keyID = keyIndex + (int)_firstKeyID;
        
        if(_keyStates[keyIndex] == KeyState::Pressed)
        {
            _components.keyboard->setPressed(keyID);
            qb::Pcm16 pcm = Modulator::attack(pitchToFreq((float)keyID), fracTime, 1.0f, 0.5f, _genTime);
            _toQueue.push_back(pcm);
            _keyStates[keyIndex] = KeyState::Maintained;
        }
        else if(_keyStates[keyIndex] == KeyState::Maintained)
        {
            _components.keyboard->setPressed(keyID);
            qb::Pcm16 pcm = Modulator::generate(pitchToFreq((float) keyID), fracTime, 0.5f, 0.0f, _genTime);
            _toQueue.push_back(pcm);
        }
        else if(_keyStates[keyIndex] == KeyState::Released)
        {
            qb::Pcm16 pcm = Modulator::release(pitchToFreq((float) keyID), fracTime, 0.5f, _genTime);
            _toQueue.push_back(pcm);
            _keyStates[keyIndex] = KeyState::None;
        }
    }
    
    if(sound._PcmDatas.size() < 1)
    {
        qb::Pcm16 pcm;
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
