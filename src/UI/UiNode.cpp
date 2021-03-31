#include "UI/UiNode.h"

#include "UI/UiConnections.h"

UiNode* UiNode::focused = nullptr;

UiNode::UiNode(const std::string& title, const vec2& position, const vec2& size)
    : UiFrame(position, size)
{
    this->title = std::make_unique<UiText>(title, vec2(5.0, 5.0), 8.0f, 0xAAAAFFFF);
    float yoft = this->title->position.y + this->title->getTextSize().y * 2.0;
    this->add(this->title.get());
}

bool UiNode::onEvent(const UiEvent& event)
{
    bool ret = false;
    float step_y = size.y / (1+inputs.size());
    float y = step_y;
    for (auto& pin : inputs)
    {
        pin.second->parentPosition = parentPosition;
        pin.second->parentClippingRect = parentClippingRect;
        pin.second->position = position + vec2(-10,y);
        ret = pin.second->onEvent(event) || ret;
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y;
    for (auto& pin : outputs)
    {
        pin.second->parentPosition = parentPosition;
        pin.second->parentClippingRect = parentClippingRect;
        pin.second->position = position + vec2(size.x,y);
        ret = pin.second->onEvent(event) || ret;
        y += step_y;
    }

    ret = UiFrame::onEvent(event) || ret;
    while(nextClicked()) focused = this;

    return ret;
}

void UiNode::draw()
{
    UiFrame::draw();
    float step_y = size.y / (1+inputs.size());
    float y = step_y;
    for (auto& pin : inputs)
    {
        pin.second->parentPosition = parentPosition;
        pin.second->parentClippingRect = parentClippingRect;
        pin.second->position = position + vec2(-10,y);
        pin.second->draw();
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y;
    for (auto& pin : outputs)
    {
        pin.second->parentPosition = parentPosition;
        pin.second->parentClippingRect = parentClippingRect;
        pin.second->position = position + vec2(size.x,y);
        pin.second->draw();
        y += step_y;
    }
}

void UiNode::addPin(int id, const std::string& label, bool isOutput)
{
    if (isOutput)
    {
        outputs[id] = std::make_unique<UiPin>(this, label, vec2(size.x,0.0), vec2(10,10));
        outputs[id]->multipleConnections = true;
        outputs[id]->textOnLeft = false;
    }
    else
    {
        inputs[id] = std::make_unique<UiPin>(this, label, vec2(-10,0.0), vec2(10,10));
        inputs[id]->multipleConnections = false;
        inputs[id]->textOnLeft = true;
    }

}

UiNode* UiNode::getSourceNode(int id)
{
    auto it = inputs.find(id);
    if (it == inputs.end() || it->second->connectionIds.size() == 0) return nullptr;

    auto other = it->second->getLinked();
    if (other != nullptr)
        return other->parentNode;
    else
        return nullptr;
}

void UiNode::displayProperties()
{
}