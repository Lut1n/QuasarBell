#include "Ui/UiText.h"

#include "Graphics/RenderInterface.h"

UiText::UiText(const std::string& text, const vec2& position, float size, unsigned color)
    : UiElement(position)
    , text(text)
    , size(size)
    , color(color)
{
}

bool UiText::onEvent(const UiEvent& event)
{
    return false;
}

void UiText::draw()
{
    if(!visible) return;
    auto& textRenderer = UiSystem::instance()->glyphRenderer;

    Rect surface = Rect::fromPosAndSize(parentPosition + position, getTextSize());
    if(parentClippingRect.outside(surface)) return;

    textRenderer.renderClippedText(text, parentClippingRect, surface.p0, size, color);
}

vec2 UiText::getTextSize() const
{
    auto& textRenderer = UiSystem::instance()->glyphRenderer;
    return textRenderer.computeSize(text, size);
}

Rect UiText::computeBounds() const
{
    return Rect::fromPosAndSize(position, getTextSize());
}
