#ifndef UI_LIST_H
#define UI_LIST_H

#include <memory>

#include "ui/UiSystem.h"

#include "ui/UiContainer.h"
#include "ui/UiRect.h"
#include "ui/UiSlider.h"

struct UiList : public UiElement
{
    UiList(const vec2& position, const vec2& size, unsigned color = 0xAAAAAAFF);
    
    void update(const UiTime& time) override;
    bool onEvent(const UiEvent& event) override;
    void draw() override;
    
    void add(UiElement* el, bool isRenderable = true, bool isListener = true);
    void rem(UiElement* el);
    
    Rect computeBounds() const override;
    
public:
    std::unique_ptr<UiContainer> container;
    std::unique_ptr<UiSlider> slider;
    
    vec2 scroll;
    vec2 size;
    unsigned color = 0x000000FF;
    bool visible = true;
};

#endif // UI_LIST_H
