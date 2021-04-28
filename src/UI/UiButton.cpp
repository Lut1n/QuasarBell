#include "ui/UiButton.h"

UiButton::UiButton(const std::string& text, const vec2& position, const vec2& size, unsigned color)
    : UiRect(position, size, color)
{
    this->text = std::make_unique<UiText>(text, vec2(0.0,0.0), 6.0f, 0x000000FF);
    this->text->position = position + (size - this->text->getTextSize()) * 0.5;
    baseColor = color;
}

bool UiButton::onEvent(const UiEvent& event)
{
    unsigned overAdditif = 0x11111100;
    unsigned pressAdditif = 0x22222200;

    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    bool mouseOver = false;
    color = baseColor;
    
    if(event.type == UiEvent::TYPE_MOUSE_MOVE)
    {
        mouseOver = surface.inside(event.position);
        if(mouseOver) color += overAdditif;
        if(pressed) color += pressAdditif;
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_1)
    {
        mouseOver = surface.inside(event.position);
        if(mouseOver) color += overAdditif;
        if(mouseOver && event.state == UiEvent::STATE_DOWN)
        {
            pressed = true;
            if(pressed) color += pressAdditif;
            return true;
        }
        else if(event.state == UiEvent::STATE_RELEASED)
        {
            if(pressed && mouseOver && clicks<10) clicks++;
            pressed = false;
            if(pressed) color += pressAdditif;
            return false;
        }
    }
    return false;
}

void UiButton::draw()
{
    UiRect::draw();
    text->position = position + (size - text->getTextSize()) * 0.5;
    text->parentPosition = parentPosition;
    text->parentClippingRect = parentClippingRect;
    text->draw();
}

bool UiButton::nextClicked()
{
    if(clicks > 0)
    {
        clicks--;
        return true;
    }
    return false;
}

Rect UiButton::computeBounds() const
{
    return Rect::fromPosAndSize(position, size);
}

UiEnumeration::UiEnumeration(const ValueList& values, const vec2& position, const vec2& size, unsigned color)
    : UiButton("", position, size, color)
{
    this->values = values;
    if(values.size() > 0)
    {
        selectedIndex = 0;
        text->text = values[selectedIndex];
    }
}

bool UiEnumeration::onEvent(const UiEvent& event)
{
    bool ret = UiButton::onEvent(event);
    while(nextClicked())
    {
        selectedIndex = (selectedIndex + 1) % values.size();
        text->text = values[selectedIndex];
    }
    return ret;
}
