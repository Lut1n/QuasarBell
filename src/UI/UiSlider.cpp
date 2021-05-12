#include "Ui/UiSlider.h"


UiSlider::UiSlider(const vec2& position, const vec2& range)
    : UiButton("-", position, vec2(20,20))
    , range(range)
{}

bool UiSlider::onEvent(const UiEvent& event)
{
    bool res = UiButton::onEvent(event);
    
    if(pressed && event.type == UiEvent::TYPE_MOUSE_MOVE)
    {
        vec2 delta = event.position - lastMousePosition;
        position.y = lastPosition.y + delta.y;
        if(position.y < range.x) position.y = range.x;
        if(position.y > range.y-size.y) position.y = range.y-size.y;
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_1)
    {
        Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
        bool mouseOver = surface.inside(event.position);

        if(mouseOver && event.state == UiEvent::STATE_DOWN)
        {
            lastPosition = position;
            lastMousePosition = event.position;
        }
    }
    return res;
}

float UiSlider::progress() const
{
    return (position.y - range.x) / (range.y-size.y - range.x);
}
