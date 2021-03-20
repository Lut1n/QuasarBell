#ifndef UI_TEXT_H
#define UI_TEXT_H

#include "UI/UiSystem.h"

struct UiText : public UiElement
{
    UiText(const std::string& text, const vec2& position = vec2(), float size = 6.0, unsigned color = 0xFFFFFFFF);
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    vec2 getTextSize() const;
    
    Rect computeBounds() const override;
    
public:
    std::string text;
    float size;
    unsigned color = 0x000000FF;
    bool visible = true;
};

#endif // UI_TEXT_H
