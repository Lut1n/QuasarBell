#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "UI/UiSystem.h"

struct UiContainer : public UiElement
{
    UiContainer(const Rect& clippingRect);
    
    void update(const UiTime& time) override;
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    void add(UiElement* el, bool isRenderable = true, bool isListener = true);
    void rem(UiElement* el);
    
    Rect computeBounds() const override;
    
public:
    Rect clippingRect;
    vec2 contentPosition;

private:
    std::vector<UiElement*> _listeners;
    std::vector<UiElement*> _elements;
    std::vector<UiElement*> _renderables;
};

#endif // UI_CONTAINER_H
