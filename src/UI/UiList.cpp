#include "ui/UiList.h"

UiList::UiList(const vec2& position, const vec2& size, unsigned color)
    : UiElement(position)
    , size(size)
    , color(color)
{
    container = std::make_unique<UiContainer>(Rect{vec2(0.0,0.0), vec2(size.x-20,size.y)});
    slider = std::make_unique<UiSlider>(vec2(size.x-20,0), vec2(0.0,size.y));
}

bool UiList::onEvent(const UiEvent& event)
{
    if(!visible) return false;
    
    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    bool clipped = parentClippingRect.outside(surface);

    if(event.type != UiEvent::TYPE_DEFOCUS && clipped) return false;

    vec2 absPosition = parentPosition + position;
    container->parentClippingRect = parentClippingRect;
    container->parentPosition = absPosition;
    container->contentPosition = scroll;
    container->clippingRect = Rect::fromPosAndSize(vec2(),size);
    if( container->onEvent(event)) return true;

    slider->parentPosition = absPosition;
    slider->parentClippingRect = parentClippingRect;
    return slider->onEvent(event);

}

void UiList::update(const UiTime& time)
{
    container->contentPosition = vec2();
    container->parentPosition = parentPosition + position;
    container->parentClippingRect = parentClippingRect;
    container->clippingRect = Rect::fromPosAndSize(vec2(),size);
    container->update(time);
}

void UiList::draw()
{
    if(!visible) return;
    
    Rect surface{parentPosition + position, parentPosition + position + size};
    if(parentClippingRect.outside(surface)) return;
    
    Rect bounds = computeBounds();

    vec2 absPosition = parentPosition + position;
    container->contentPosition = scroll;
    container->parentClippingRect = parentClippingRect;
    container->parentPosition = absPosition;
    container->clippingRect = Rect::fromPosAndSize(vec2(),size);
    container->draw();
    
    slider->parentPosition = absPosition;
    slider->parentClippingRect = parentClippingRect;
    slider->draw();

    float d = bounds.size().y - size.y;
    scroll.y = -slider->progress() * (d > 0.0 ? d : 0.0);
}

void UiList::add(UiElement* el, bool isRenderable, bool isListener)
{
    container->add(el, isRenderable, isListener);
}

void UiList::rem(UiElement* el)
{
    container->rem(el);
}

Rect UiList::computeBounds() const
{
    Rect bounds = container->computeBounds();
    return bounds;
}
