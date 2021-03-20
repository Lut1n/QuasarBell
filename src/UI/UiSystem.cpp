#include "UI/UiSystem.h"

#include <algorithm>

#include "UI/UiConnections.h"

static UiSystem s_uiSystemSingleton;

UiElement::UiElement(const vec2& position)
    : position(position)
{
}

UiSystem::UiSystem()
{
}

void UiSystem::initialize()
{
    glyphRenderer.initialize();
    // glyphRenderer.save();
}

void UiSystem::setActivated(bool activated)
{
    _activated = activated;
}

bool UiSystem::isActivated() const
{
    return _activated;
}

bool UiSystem::onEvent(const UiEvent& event)
{
    if(!_activated) return false;

    Rect clippingRect = {vec2(), vec2(10000.0, 10000.0)};
    bool captured = false;
    for(auto it = _listeners.rbegin(); it != _listeners.rend(); it++)
    {
        auto& el = *it;
        el->parentClippingRect = clippingRect;
        captured = el->onEvent(event);
        if(captured) break;
    }

    if(!captured && event.type == UiEvent::TYPE_MOUSE_BUTTON)
    {
        if(UiConnections::instance)
            UiConnections::instance->abortLink();
    }
    return captured;
}

void UiSystem::update(const UiTime& time)
{
    if(!_activated) return;

    Rect clippingRect = {vec2(), vec2(10000.0, 10000.0)};
    for(auto el : _elements)
    {
        el->parentClippingRect = clippingRect;
        el->update(time);
    }
}

void UiSystem::draw()
{
    if(!_activated) return;
    
    Rect clippingRect = {vec2(), vec2(10000.0, 10000.0)};
    for(auto el : _renderables)
    {
        el->parentClippingRect = clippingRect;
        el->draw();
    }
}

void UiSystem::add(UiElement* el, bool isRenderable, bool isListener)
{
    _elements.push_back(el);
    if(isRenderable) _renderables.push_back(el);
    if(isListener) _listeners.push_back(el);
}

void UiSystem::rem(UiElement* el)
{
    auto eraseFrom = [](auto& arr, UiElement* el) {
        auto found = std::find(arr.begin(), arr.end(), el);
        if(found != arr.end()) arr.erase(found);
    };
    
    eraseFrom(_elements, el);
    eraseFrom(_renderables, el);
    eraseFrom(_listeners, el);
}

UiSystem* UiSystem::instance()
{
    return &s_uiSystemSingleton;
}
