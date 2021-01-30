#ifndef QUASAR_BELL_KEYBOARD_HPP
#define QUASAR_BELL_KEYBOARD_HPP

#include <list>
#include <vector>

#include "signal/PcmData.hpp"
#include "gui/Gui.hpp"

//--------------------------------------------------------------
class Keyboard : public GuiModule
{
public:
    enum KeyState
    {
        None,
        Pressed,
        Maintained,
        Released
    };
    
    struct Event
    {
        int id;
        KeyState state;
    };
    
    struct KeyView
    {
        int id;
        float oft;
        unsigned color;
        bool black;
        bool pressed;
    };
    
    Keyboard(unsigned firstId = 21, unsigned lastId = 108);
    
    void reset();
    void setPressed(int i);
    
    bool isOverKey(float x, float y, const KeyView& key);
    void pushEvent(int keyid, KeyState state);
    bool pushEventIfPressed(KeyView& key, float x, float y, bool mouseDown);
    void processEvents();
    void draw();
    void render() override;

public:
    std::list<Event> events;

private:
    unsigned _keyFirstIndex = 21;
    unsigned _keyLastIndex = 108;
    
    int _lastKeyPressed = 0;
    
    std::vector<bool> _keys;
    std::vector<KeyView> _whiteKeys;
    std::vector<KeyView> _blackKeys;

    float _keyWidth;
    float _keyHeight;
};

#endif // QUASAR_BELL_KEYBOARD_HPP
