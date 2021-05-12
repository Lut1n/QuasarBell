#include "Ui/UiNode.h"

#include "Ui/UiConnections.h"

std::list<UiNode*> UiNode::selected;

bool UiNode::isSelected(UiNode* node)
{
    for (auto n : selected)
        if (n == node) return true;
    return false;
}

UiNode::UiNode(const std::string& title, const vec2& position, const vec2& size)
    : UiFrame(position, size)
{
    this->title = std::make_unique<UiText>(title, vec2(5.0, 5.0), 6.0f, 0xAAAAFFFF);
    float yoft = this->title->position.y + this->title->getTextSize().y * 2.f;
    this->add(this->title.get());
    borderEnabled = false;
    color = colorOnIdle;
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
    for (auto it = selected.begin(); it != selected.end();)
    {
        if (*it == this)
        {
            it = selected.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void UiNode::startMove(const vec2& mousePos)
{
    if (UiNode::isSelected(this))
    {
        for(auto n : selected) n->startMovePosition = n->position;
    }
    else
    {
        UiFrame::startMove(mousePos);
    }
}

void UiNode::endMove(const vec2& mousePos)
{
    if (!UiNode::isSelected(this))
    {
        UiFrame::endMove(mousePos);
    }
}

void UiNode::onMove(const vec2& delta)
{
    if (UiNode::isSelected(this))
    {
        for(auto n : selected) n->position = n->startMovePosition + delta;
    }
    else
    {
        UiFrame::onMove(delta);
    }
}

bool UiNode::onEvent(const UiEvent& event)
{
    static const float PinSize = 10.f;
    bool ret = false;
    float step_y = size.y / (1+inputs.size());
    float y = step_y - PinSize * 0.5f;
    for (auto& pin : inputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(-10,y);
        ret = pin->onEvent(event) || ret;
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y - PinSize * 0.5f;
    for (auto& pin : outputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(size.x,y);
        ret = pin->onEvent(event) || ret;
        y += step_y;
    }

    ret = UiFrame::onEvent(event) || ret;

    while(nextClicked())
    {
        if (!UiNode::isSelected(this))
        {
            if (!UiSystem::instance()->multiselectionEnabled() && !UiNode::selected.empty())
                UiNode::selected.clear();
            selected.push_back(this);
        }
    }
    while(nextRClicked()) toDelete = true;

    return ret;
}

void UiNode::draw()
{
    static const float PinSize = 10.0;
    borderEnabled = UiNode::isSelected(this);
    UiFrame::draw();

    Rect previewArea;
    float padding = 5.0;
    previewArea.p0 = vec2(padding, padding + title->position.y + title->getTextSize().y);
    previewArea.p1 = size - vec2(padding, padding);
    drawPreview(previewArea);

    float step_y = size.y / (1+inputs.size());
    float y = step_y - PinSize * 0.5f;
    for (auto& pin : inputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(-10,y);
        pin->draw();
        y += step_y;
    }
    step_y = size.y / (1+outputs.size());
    y = step_y - PinSize * 0.5f;
    for (auto& pin : outputs)
    {
        pin->parentPosition = parentPosition;
        pin->parentClippingRect = parentClippingRect;
        pin->position = position + vec2(size.x,y);
        pin->draw();
        y += step_y;
    }
}

void UiNode::drawPreview(const Rect& previewArea)
{
}

void UiNode::addPin(int id, const std::string& label, bool isOutput)
{
    static const float PinSize = 10.0;
    if (isOutput)
    {
        if(outputs.size() <= id) outputs.resize(id+1);
        outputs[id] = std::make_unique<UiPin>(this, label, vec2(size.x,0.0), vec2(PinSize,PinSize));
        outputs[id]->isInput = false;
    }
    else
    {
        if(inputs.size() <= id) inputs.resize(id+1);
        inputs[id] = std::make_unique<UiPin>(this, label, vec2(-10,0.0), vec2(PinSize,PinSize));
        inputs[id]->isInput = true;
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