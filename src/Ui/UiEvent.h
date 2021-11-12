#ifndef UI_EVENT_H
#define UI_EVENT_H

#include "Core/Vec2.h"

struct UiEvent
{
    enum Type
    {
        TYPE_NONE = 0,
        TYPE_MOUSE_BUTTON,
        TYPE_MOUSE_MOVE,
        TYPE_KEY,
        TYPE_CHAR,
        TYPE_DEFOCUS
    };

    enum Input
    {
        INPUT_NONE = 0,
        INPUT_MOUSE_1,
        INPUT_MOUSE_2,
        INPUT_MOUSE_3
    };
    
    enum State
    {
        STATE_NONE = 0,
        STATE_DOWN,
        STATE_RELEASED,
        STATE_SCROLL
    };
    
    unsigned type = TYPE_NONE;
    unsigned input = INPUT_NONE;
    unsigned state = STATE_NONE;
    vec2 position;
};

#endif // UI_EVENT_H
