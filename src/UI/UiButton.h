#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <memory>

#include "UI/UiSystem.h"

#include "UI/UiRect.h"
#include "UI/UiText.h"

struct UiButton : public UiRect
{
    UiButton(const std::string& text, const vec2& position, const vec2& size, unsigned color = 0xCCCCCCFF);
    
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    bool nextClicked();
    
    Rect computeBounds() const override;
    
public:
    std::unique_ptr<UiText> text;
    unsigned baseColor = 0xFFFFFFFF;
    bool pressed = false;
    int clicks = 0;
};

struct UiEnumeration : public UiButton
{
    using ValueList = std::vector<std::string>;
    UiEnumeration(const ValueList& values, const vec2& position, const vec2& size, unsigned color = 0xCCCCCCFF);
    
    bool onEvent(const UiEvent& event) override;
    
public:
    ValueList values;
    int selectedIndex = -1;
};

#endif // UI_BUTTON_H
