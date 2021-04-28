#ifndef UI_RECT_H
#define UI_RECT_H

#include "ui/UiSystem.h"

struct UiRect : public UiElement
{
    UiRect(const vec2& position, const vec2& size, unsigned color = 0xAAAAAAFF);
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;
    
    Rect computeBounds() const override;
    
public:
    vec2 size;
    unsigned color = 0x000000FF;
    bool visible = true;
    bool borderEnabled = true;
};

#endif // UI_RECT_H
