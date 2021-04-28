#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include <vector>

#include "core/Vec2.h"
#include "font/GlyphRenderer.h"

#include "ui/UiEvent.h"

struct UiTime
{
    double fromStart = 0.0;
    double delta = 0.0;
};

struct UiElement
{
    UiElement(const vec2& position);
    virtual ~UiElement() = default;
    
    virtual void update(const UiTime& time) {};
    virtual bool onEvent(const UiEvent& event) = 0;
    virtual void draw() = 0;
    
    virtual Rect computeBounds() const = 0;

    vec2 parentPosition;
    Rect parentClippingRect;
    vec2 position;
};

struct UiSystem
{
    UiSystem();
    
    void initialize();
    
    bool onEvent(const UiEvent& event);
    void update(const UiTime& time);
    void draw();
    
    void add(UiElement* el, bool isRenderable = true, bool isListener = false);
    void rem(UiElement* el);
    
    static UiSystem* instance();

    void setActivated(bool activated);
    bool isActivated() const;

    bool multiselectionEnabled() const;
    
public:
    GlyphRenderer glyphRenderer;
private:
    std::vector<UiElement*> _listeners;
    std::vector<UiElement*> _elements;
    std::vector<UiElement*> _renderables;
    bool _activated = false;
    bool _shiftKey = false;
};

#endif // UI_SYSTEM_H
