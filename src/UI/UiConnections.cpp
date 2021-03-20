#include "UI/UiConnections.h"

#include "GL/RenderInterface.h"
#include "UI/UiPin.h"

UiConnections* UiConnections::instance = nullptr;

UiConnections::UiConnections()
    : UiElement(vec2(0.0,0.0))
{
    instance = this;
}

std::uint64_t UiConnections::createLink(UiPin* a, UiPin* b)
{
    if(a == b || a->type != b->type || a->textOnLeft == b->textOnLeft) return 0;

    std::uint64_t id = previousId++;
    links[id] = Link{a,b};
    if (a->connectionIds.size() > 0 && !a->multipleConnections)
    {
        for (auto id : a->connectionIds) deleteLink(id);
        a->connectionIds.clear();
    }
    if (b->connectionIds.size() > 0 && !b->multipleConnections)
    {
        for (auto id : b->connectionIds) deleteLink(id);
        b->connectionIds.clear();
    }
    a->addConnectionId(id);
    b->addConnectionId(id);
    return id;
}

void UiConnections::updateLink(std::uint64_t id, UiPin* a, UiPin* b)
{
    links[id] = Link{a,b};
}

void UiConnections::deleteLink(std::uint64_t id)
{
    auto& link = links[id];

    link.first->remConnectionId(id);
    link.second->remConnectionId(id);
    links.erase(id);
}

std::uint64_t UiConnections::startLink(UiPin* a)
{
    partialLink.first = a;
    partialLink.second = a->parentPosition + a->position + a->size * 0.5;
    displayPartial = true;
    currentId = previousId+1;
    return currentId;
}

void UiConnections::updateCurrentLink(vec2 b)
{
    if(displayPartial)
        partialLink.second = b;
}

std::uint64_t UiConnections::endLink(UiPin* b)
{
    std::uint64_t id = 0;
    if (displayPartial)
    {
        id = createLink(partialLink.first, b);
        partialLink.first = nullptr;
        displayPartial = false;
        currentId = 0;
    }
    return id;
}

void UiConnections::abortLink()
{
    partialLink.first = nullptr;
    displayPartial = false;
    currentId = 0;
}

bool UiConnections::onEvent(const UiEvent& event)
{
    if(event.type == UiEvent::TYPE_MOUSE_MOVE)
    {
        updateCurrentLink(event.position);
    }
    return false;
}

void UiConnections::update(const UiTime& time)
{

}

Rect UiConnections::computeBounds() const
{
    return Rect();
}

void UiConnections::draw()
{
    for(auto& link : links)
    {
        auto& pair = link.second;
        vec2 p1 = pair.first->parentPosition + pair.first->position + pair.first->size * 0.5;
        vec2 p2 = pair.second->parentPosition + pair.second->position + pair.second->size * 0.5;

        RenderInterface::setColor(0xFFFFFFFF);
        RenderInterface::setThickness(2.0f);
        RenderInterface::line(p1, p2);
    }

    if (displayPartial)
    {
        vec2 p1 = partialLink.first->parentPosition + partialLink.first->position + partialLink.first->size * 0.5;

        RenderInterface::setColor(0xFFFFFFFF);
        RenderInterface::setThickness(2.0f);
        RenderInterface::line(p1, partialLink.second);
    }
}
