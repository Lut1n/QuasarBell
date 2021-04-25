#ifndef UI_FRAME_H
#define UI_FRAME_H

#include <memory>

#include "UI/UiSystem.h"

#include "UI/UiRect.h"
#include "UI/UiContainer.h"

struct UiFrame : public UiRect
{
    UiFrame(const vec2& position, const vec2& size);
    
    void update(const UiTime& time) override;
    bool onEvent(const UiEvent& event) override;
    void draw() override;

    bool nextClicked();
    bool nextRClicked();

    void add(UiElement* el, bool isRenderable = true, bool isListener = true);
    void rem(UiElement* el);
    
protected:

    virtual void startMove(const vec2& mousePos);
    virtual void endMove(const vec2& mousePos);
    virtual void onMove(const vec2& delta);

public:
    bool moving = false;
    vec2 lastMousePosition;
    vec2 lastPosition;
    vec2 startMovePosition;
    std::unique_ptr<UiContainer> children;
    bool pressed = false;
    bool rpressed = false;
    int clicks = 0;
    int rclicks = 0;
};

#endif // UI_FRAME_H
