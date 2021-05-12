#ifndef UI_INPUT_H
#define UI_INPUT_H

#include <memory>

#include "Ui/UiSystem.h"

#include "Ui/UiText.h"
#include "Ui/UiRect.h"

struct UiInput : public UiRect
{
    UiInput(const std::string& text, const vec2& position, const vec2& size);
    
    void update(const UiTime& time) override;
    bool onEvent(const UiEvent& event) override;
    void draw() override;
    
public:
    std::unique_ptr<UiText> text;
    std::string baseText;
    bool isFocus = false;
    bool pressed = false;
    unsigned baseColor = 0xAAAAAAFF;
    double elapsed = 0.0;
};

#endif // UI_INPUT_H
