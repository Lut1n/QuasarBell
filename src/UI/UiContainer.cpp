#include "UI/UiContainer.h"

#include <algorithm>

UiContainer::UiContainer(const Rect& clippingRect)
    : UiElement(vec2())
    , clippingRect(clippingRect)
{
}

bool UiContainer::onEvent(const UiEvent& event)
{
    // clipping
    Rect absClippingRect = clippingRect + parentPosition;
    absClippingRect = absClippingRect.clampTo(parentClippingRect);
    vec2 absPosition = parentPosition + contentPosition;
    if(event.type == UiEvent::TYPE_MOUSE_MOVE || event.type == UiEvent::TYPE_MOUSE_BUTTON)
    {
        bool mouseOver = absClippingRect.inside(event.position);
        if(!mouseOver) return false;
    }

    bool captured = false;
    for(auto it = _listeners.rbegin(); it != _listeners.rend(); it++)
    {
        auto& el = *it;
        el->parentClippingRect = absClippingRect;
        el->parentPosition = absPosition;
        captured = el->onEvent(event);
        if(captured) break;
    }
    return captured;
}

void UiContainer::update(const UiTime& time)
{
    Rect absClippingRect = clippingRect + parentPosition;
    absClippingRect = absClippingRect.clampTo(parentClippingRect);
    vec2 absPosition = parentPosition + contentPosition;
    for(auto el : _elements)
    {
        el->parentClippingRect = absClippingRect;
        el->parentPosition = absPosition;
        el->update(time);
    }
}

void UiContainer::draw()
{
    Rect absClippingRect = clippingRect + parentPosition;
    absClippingRect = absClippingRect.clampTo(parentClippingRect);
    vec2 absPosition = parentPosition + contentPosition;
    for(auto el : _renderables)
    {
        el->parentClippingRect = absClippingRect;
        el->parentPosition = absPosition;
        el->draw();
    }
}

void UiContainer::add(UiElement* el, bool isRenderable, bool isListener)
{
    _elements.push_back(el);
    if(isRenderable) _renderables.push_back(el);
    if(isListener) _listeners.push_back(el);
}

void UiContainer::rem(UiElement* el)
{
    auto eraseFrom = [](auto& arr, UiElement* el) {
        auto found = std::find(arr.begin(), arr.end(), el);
        if(found != arr.end()) arr.erase(found);
    };
    
    eraseFrom(_elements, el);
    eraseFrom(_renderables, el);
    eraseFrom(_listeners, el);
}

Rect UiContainer::computeBounds() const
{
    Rect bounds;
    for(auto el : _elements)
    {
        bounds = bounds.extends(el->computeBounds());
    }
    return bounds;
}
