#include "UI/UiFrame.h"

UiFrame::UiFrame(const vec2& position, const vec2& size)
    : UiRect(position, size, 0x323233FF)
{
    this->children = std::make_unique<UiContainer>(Rect::fromPosAndSize(vec2(),size));
}

bool UiFrame::onEvent(const UiEvent& event)
{
    children->contentPosition = vec2();
    children->parentPosition = parentPosition + position;
    children->parentClippingRect = parentClippingRect;
    children->clippingRect = Rect::fromPosAndSize(vec2(),size);
    if(children->onEvent(event)) return true;

    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    if(event.type == UiEvent::TYPE_MOUSE_MOVE)
    {
        bool mouseOver = surface.inside(event.position);
        
        color = mouseOver ? 0x3A3A3BFF : 0x323233FF;
        if(pressed || moving)
        {
            moving = true;
            vec2 delta = event.position - lastMousePosition;
            position = lastPosition + delta;
        }
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_1)
    {
        bool mouseOver = surface.inside(event.position);
        
        if(mouseOver && event.state == UiEvent::STATE_DOWN)
        {
            pressed = true;
            lastPosition = position;
            lastMousePosition = event.position;
            return true;
        }
        else if(event.state == UiEvent::STATE_RELEASED)
        {
            if(!moving && pressed && mouseOver && clicks<10) clicks++;
            pressed = false;
            moving = false;
        }
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_2)
    {
        bool mouseOver = surface.inside(event.position);
        
        if(mouseOver && event.state == UiEvent::STATE_DOWN)
        {
            rpressed = true;
            lastPosition = position;
            lastMousePosition = event.position;
            return true;
        }
        else if(event.state == UiEvent::STATE_RELEASED)
        {
            if(rpressed && mouseOver && rclicks<10)
            {
                rclicks++;    
                rpressed = false;
                return true;
            }
            rpressed = false;
        }
    }
    return false;
}

void UiFrame::update(const UiTime& time)
{
    UiRect::update(time);
    children->contentPosition = vec2();
    children->parentPosition = parentPosition + position;
    children->parentClippingRect = parentClippingRect;
    children->clippingRect = Rect::fromPosAndSize(vec2(),size);
    children->update(time);
}

void UiFrame::draw()
{
    UiRect::draw();
    children->contentPosition = vec2();
    children->parentPosition = parentPosition + position;
    children->parentClippingRect = parentClippingRect;
    children->clippingRect = Rect::fromPosAndSize(vec2(),size);
    children->draw();
}

void UiFrame::add(UiElement* el, bool isRenderable, bool isListener)
{
    children->add(el, isRenderable, isListener);
}

void UiFrame::rem(UiElement* el)
{
    children->rem(el);
}

bool UiFrame::nextClicked()
{
    if(clicks > 0)
    {
        clicks--;
        return true;
    }
    return false;
}

bool UiFrame::nextRClicked()
{
    if(rclicks > 0)
    {
        rclicks--;
        return true;
    }
    return false;
}