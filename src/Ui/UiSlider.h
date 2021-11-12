#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include <memory>

#include "Ui/UiSystem.h"

#include "Ui/UiButton.h"


struct UiSlider : public UiButton
{
    vec2 lastMousePosition;
    vec2 lastPosition;
    vec2 range;

    UiSlider(const vec2& position, const vec2& range);
    
    bool onEvent(const UiEvent& event) override;

    float progress() const;
};

#endif // UI_SLIDER_H