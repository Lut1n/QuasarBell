#ifndef UI_FORM_H
#define UI_FORM_H

#include <memory>

#include "Ui/UiSystem.h"
#include "Ui/UiFrame.h"
#include "Ui/UiList.h"
#include "Ui/UiText.h"

struct UiForm : public UiFrame
{
    enum Layout
    {
        Column,
        Line,
        Free
    };

    UiForm(const std::string& title, const vec2& position, const vec2& size);
    
    void push(UiElement* el, Layout layout = Column, bool isRenderable = true, bool isListener = true);

public:
    std::unique_ptr<UiList> list;
    std::unique_ptr<UiText> title;

    vec2 cursor;
    vec2 previousSize;
};

#endif // UI_FORM_H
