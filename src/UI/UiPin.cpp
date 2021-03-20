#include "UI/UiPin.h"

#include <algorithm>

#include "UI/UiConnections.h"

UiPin::UiPin(UiNode* node, const std::string& label, const vec2& position, const vec2& size)
    : UiRect(position, size, 0x11AAAAFF)
{
    parentNode = node;
    this->label = std::make_unique<UiText>(label, position, 6.0f, 0xFFFFFFFF);
}

UiPin::~UiPin()
{
    if (UiConnections::instance == nullptr) return;
    for (auto id : connectionIds)
        UiConnections::instance->deleteLink(id);
}

bool UiPin::onEvent(const UiEvent& event)
{
    Rect surface = Rect::fromPosAndSize(parentPosition + position, size);
    if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_1)
    {
        bool mouseOver = surface.inside(event.position);
        if(event.state == UiEvent::STATE_DOWN)
        {
            if (mouseOver)
            {
                UiConnections::instance->startLink(this);
                return true;
            }
            
        }
        else if(event.state == UiEvent::STATE_RELEASED)
        {
            if (mouseOver)
                UiConnections::instance->endLink(this);
        }
    }
    else if(event.type == UiEvent::TYPE_MOUSE_BUTTON && event.input == UiEvent::INPUT_MOUSE_2)
    {
        bool mouseOver = surface.inside(event.position);
        if(event.state == UiEvent::STATE_DOWN)
        {
            if (mouseOver)
            {
                for (auto id : connectionIds)
                    UiConnections::instance->deleteLink(id);
                return true;
            }
        }
    }
    return false;
}

void UiPin::draw()
{
    UiRect::draw();
    this->label->parentClippingRect = parentClippingRect;
    this->label->parentPosition = parentPosition;
    this->label->position = position + (textOnLeft ? vec2(-this->label->getTextSize().x - 5.0, 0.0) : vec2(size.x + 5.0, 0.0));
    this->label->draw();
}

void UiPin::addConnectionId(std::uint64_t id)
{
    auto it = std::find(connectionIds.begin(), connectionIds.end(), id);
    if (it == connectionIds.end())
        connectionIds.push_back(id);
}

void UiPin::remConnectionId(std::uint64_t id)
{
    auto it = std::find(connectionIds.begin(), connectionIds.end(), id);
    if (it != connectionIds.end())
        connectionIds.erase(it);
}

UiPin* UiPin::getLinked()
{
    if (connectionIds.size() == 0)
        return nullptr;

    std::uint64_t firstId = connectionIds[0];
    auto& link = UiConnections::instance->links[firstId];
    if (link.first == this)
        return link.second;
    else if (link.second == this)
        return link.first;
    return nullptr;
}