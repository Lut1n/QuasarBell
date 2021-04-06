#include "UI/UiNode.h"

#include "UI/UiConnections.h"

UiNode* UiNode::focused = nullptr;

UiNode::UiNode(const std::string& title, const vec2& position, const vec2& size)
    : UiFrame(position, size)
{
    this->title = std::make_unique<UiText>(title, vec2(5.0, 5.0), 6.0f, 0xAAAAFFFF);
    float yoft = this->title->position.y + this->title->getTextSize().y * 2.0;
    this->add(this->title.get());
}

UiNode::~UiNode()
{
    /*for (auto& pin : inputs)
    {
        for (auto id : pin->connectionIds)
            UiConnections::instance->deleteLink(id);
    }
    for (auto& pin : outputs)
    {
        for (auto id : pin->connectionIds)
            UiConnections::instance->deleteLink(id);
    }*/
    if (focused == this) focused = nullptr;
}

bool UiNode::onEvent(const UiEvent& event)
{
    bool ret = false;
    float step_y = size.y / (1+inputs.size());
    float y = step_y;
    for (auto& pin : inputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(-10,y);
        ret = pin->onEvent(event) || ret;
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y;
    for (auto& pin : outputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(size.x,y);
        ret = pin->onEvent(event) || ret;
        y += step_y;
    }

    ret = UiFrame::onEvent(event) || ret;

    while(nextClicked()) focused = this;
    while(nextRClicked()) toDelete = true;

    return ret;
}

void UiNode::draw()
{
    int prev_color = color;
    if( focused == this ) color = 0x3E3E3FFF;
    UiFrame::draw();
    color = prev_color;
    float step_y = size.y / (1+inputs.size());
    float y = step_y;
    for (auto& pin : inputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(-10,y);
        pin->draw();
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y;
    for (auto& pin : outputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(size.x,y);
        pin->draw();
        y += step_y;
    }
}

void UiNode::addPin(int id, const std::string& label, bool isOutput)
{
    if (isOutput)
    {
        if(outputs.size() <= id) outputs.resize(id+1);
        outputs[id] = std::make_unique<UiPin>(this, label, vec2(size.x,0.0), vec2(10,10));
        outputs[id]->multipleConnections = true;
        outputs[id]->textOnLeft = false;
    }
    else
    {
        if(inputs.size() <= id) inputs.resize(id+1);
        inputs[id] = std::make_unique<UiPin>(this, label, vec2(-10,0.0), vec2(10,10));
        inputs[id]->multipleConnections = false;
        inputs[id]->textOnLeft = true;
    }

}

UiNode* UiNode::getSourceNode(int id)
{
    if(inputs.size() <= id) return nullptr;

    auto other = inputs[id]->getLinked();
    if (other != nullptr)
        return other->parentNode;
    else
        return nullptr;
}

void UiNode::displayProperties()
{
}

size_t UiNode::getIndex(UiPin* pin) const
{
    for (size_t i = 0; i<inputs.size(); ++i)
        if (inputs[i].get() == pin) return i;
    for (size_t i = 0; i<outputs.size(); ++i)
        if (outputs[i].get() == pin) return i;
    return 0;
}