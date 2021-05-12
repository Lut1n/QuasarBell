#include "Ui/UiForm.h"

UiForm::UiForm(const std::string& title, const vec2& position, const vec2& size)
    : UiFrame(position, size)
{
    this->title = std::make_unique<UiText>(title, vec2(5.0,5.0), 8.0f, 0xAAAAFFFF);
    float yoft = this->title->position.y + this->title->getTextSize().y * 2;
    this->add(this->title.get());
    this->list = std::make_unique<UiList>(vec2(0.0, yoft), size - vec2(0.0, yoft), 0xFF0000FF);
    this->add(this->list.get());

    cursor = vec2(5.0, 5.0);
}

void UiForm::push(UiElement* el, Layout layout, bool isRenderable, bool isListener)
{
    float margin = 5.0;
    if(layout == Column)
    {
        el->position = cursor + vec2(0.0, previousSize.y);
        cursor = el->position;
        previousSize = el->computeBounds().size() + vec2(margin,margin);
    }
    else if(layout == Line)
    {
        el->position = cursor + vec2(previousSize.x, 0.0);
        cursor = el->position;
        previousSize = el->computeBounds().size() + vec2(margin,margin);
    }
    this->list->add(el, isRenderable, isListener);
}