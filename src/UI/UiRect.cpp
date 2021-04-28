#include "ui/UiRect.h"

#include "graphics/RenderInterface.h"

UiRect::UiRect(const vec2& position, const vec2& size, unsigned color)
    : UiElement(position)
    , size(size)
    , color(color)
{
}

bool UiRect::onEvent(const UiEvent& event)
{
    return false;
}

void UiRect::draw()
{
    if(!visible) return;
    
    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    bool clipped = parentClippingRect.outside(surface);
    if(clipped) return;

    surface = surface.clampTo(parentClippingRect);
    
    RenderInterface::setColor(color);
    RenderInterface::fill(surface.p0, surface.p1);
    if (borderEnabled)
    {
        RenderInterface::setColor(0xFFFFFFFF);
        RenderInterface::setThickness(2.0f);
        RenderInterface::rect(surface.p0, surface.p1);
    }
}


Rect UiRect::computeBounds() const
{
    return Rect::fromPosAndSize(position, size);
}
