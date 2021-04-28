#ifndef QUASAR_BELL_SYNTH_H
#define QUASAR_BELL_SYNTH_H

#include <array>
#include <map>

#include "App.hpp"
#include "gui/Keyboard.hpp"

#include "signal/Signal.hpp"

//--------------------------------------------------------------
class SynthComponentGroup : public GuiComponentGroup
{
public:
    SynthComponentGroup();
    virtual ~SynthComponentGroup();

public:
    Keyboard* keyboard;
};

//--------------------------------------------------------------
class SynthWorkSpace : public ToolWorkSpace
{
public:
    enum KeyState
    {
        None,
        Pressed,
        Maintained,
        Released
    };

    void init(App* app) override;
    void onEvent(const KeyEvent& event) override;
    void update(double t) override;
    void render() override;

private:
    void _onEventImpl(int keyId, bool pressed);

private:
    std::array<KeyState, 88> _keyStates;
    std::map<char, int> _keyMap;
    std::list<qb::Pcm16> _toQueue;

    SynthComponentGroup _components;
    App* _app = nullptr;
    float _genTime = 0.0;
    
    const size_t _firstKeyID = 21;
};

#endif // QUASAR_BELL_SYNTH_H
