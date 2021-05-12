#include "Ui/UiInput.h"


UiInput::UiInput(const std::string& text, const vec2& position, const vec2& size)
    : UiRect(position, size, 0x111111FF)
{
    baseColor = 0xAAAAAAFF;
    baseText = text;
    this->text = std::make_unique<UiText>(text, position, 6.f, baseColor);
}

bool UiInput::onEvent(const UiEvent& event)
{
    bool ret = UiRect::onEvent(event);
    
    unsigned overAdditif = 0x11111100;
    unsigned pressAdditif = 0x22222200;

    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    bool mouseOver = false;
    text->color = baseColor;

    if(event.type == UiEvent::TYPE_MOUSE_MOVE)
    {
        mouseOver = surface.inside(event.position);
        if(mouseOver) text->color += overAdditif;
        if(pressed) text->color += pressAdditif;
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_1)
    {
        mouseOver = surface.inside(event.position);
        if(mouseOver) text->color += overAdditif;
        if(mouseOver && event.state == UiEvent::STATE_DOWN)
        {
            pressed = true;
            if(pressed) text->color += pressAdditif;
            return true;
        }
        else if(event.state == UiEvent::STATE_RELEASED)
        {
            if(mouseOver)
            {
                isFocus = true;
                text->text = baseText + "/";
            }
            else
            {
                isFocus = false;
                text->text = baseText;
            }
            pressed = false;
            if(pressed) text->color += pressAdditif;
            return false;
        }
    }
    else if (event.type == UiEvent::TYPE_DEFOCUS)
    {
        mouseOver = surface.inside(event.position);
        if (!mouseOver)
        {
            isFocus = false;
            text->text = baseText;
        }
        return false;
    }
    
    if(isFocus && event.type == UiEvent::TYPE_KEY && event.state == UiEvent::STATE_DOWN)
    {
        if(event.input == 259)
        {
            if(baseText.size() > 0) baseText.pop_back();
            text->text = baseText + "/";
        }
    }
    if(isFocus && event.type == UiEvent::TYPE_CHAR)
    {
        baseText += event.input;
        text->text = baseText + "/";
        
        return true;
    }
    
    return ret;
}

void UiInput::update(const UiTime& time)
{
    elapsed = time.fromStart * 2;
}

void UiInput::draw()
{
    UiRect::draw();

    unsigned pressAdditif = 0x22222200;
    text->color = baseColor;
    text->text = baseText;
    if(isFocus && (int(elapsed) % 2) == 0)
    {
        text->color += pressAdditif;
        text->text += "/";
    }
    text->position = position + vec2(5.f, (size.y - text->getTextSize().y) * 0.5f);
    text->parentPosition = parentPosition;
    text->parentClippingRect = parentClippingRect.clampTo(Rect::fromPosAndSize(parentPosition + position, size));
    text->draw();
}
