#include "Ui/UiConnections.h"

#include "Graphics/RenderInterface.h"
#include "Ui/UiPin.h"
#include "Ui/UiNode.h"

UiConnections* UiConnections::instance = nullptr;

static void findAncestors(UiNode* node, std::vector<UiNode*>& ancestors)
{
    if (node == nullptr) return;
    ancestors.push_back(node);
    for (auto& pin : node->inputs)
    {
        UiPin* linked = pin->getLinked();
        if (linked)
            findAncestors(linked->parentNode, ancestors);
    }
}

static bool cycleCheck(UiPin* a, UiPin* b)
{
    if (a->isInput)
    {
        UiPin* t = a;
        a = b;
        b = t;
    }

    std::vector<UiNode*> ancestors;
    findAncestors(a->parentNode, ancestors);
    for (auto a : ancestors)
        if (a == b->parentNode) return true;
    return false;
}

UiConnections::UiConnections()
    : UiElement(vec2(0.0,0.0))
{
    instance = this;
}

std::uint64_t UiConnections::createLink(UiPin* a, UiPin* b)
{
    if(a == b || a->type != b->type || a->isInput == b->isInput) return 0;

    if (cycleCheck(a, b))
    {
        std::cout << "Warning: Link canceled. Cycle detected" << std::endl;
        return 0;
    }

    std::uint64_t id = previousId++;
    links[id] = Link{a,b};
    if (a->connectionIds.size() > 0 && a->isInput)
    {
        for (auto id : a->connectionIds) deleteLink(id);
        a->connectionIds.clear();
    }
    if (b->connectionIds.size() > 0 && b->isInput)
    {
        for (auto id : b->connectionIds) deleteLink(id);
        b->connectionIds.clear();
    }
    a->addConnectionId(id);
    b->addConnectionId(id);
    if(handler) handler->onConnect(a,b);
    return id;
}

void UiConnections::updateLink(std::uint64_t id, UiPin* a, UiPin* b)
{
    links[id] = Link{a,b};
}

void UiConnections::deleteLink(std::uint64_t id)
{
    if (links.find(id) == links.end()) return;

    auto& link = links[id];
    std::cout << "destroy link " << id << "[" << link.first->label->text << " - " << link.second->label->text << std::endl;
    if(handler) handler->onDisconnect(link.first,link.second);

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
        drawLink(p1, p2);
    }

    if (displayPartial)
    {
        vec2 p1 = partialLink.first->parentPosition + partialLink.first->position + partialLink.first->size * 0.5;

        RenderInterface::setColor(0xFFFFFFFF);
        RenderInterface::setThickness(2.0f);
        drawLink(p1, partialLink.second);
    }
}

void UiConnections::drawLink(vec2 p1, vec2 p2)
{
    const size_t pointCount = 8;
    auto swapv = [](vec2& v1, vec2& v2) { vec2 t=v1; v1=v2; v2=t; };
    if(p2.x < p1.x) swapv(p1, p2);

    vec2 l1 = p1;
    for(size_t i=0; i<=pointCount; ++i)
    {
        float x = (float)i / pointCount;
        float y = x * x * (3.f - 2.f * x);
        vec2 l2;
        l2.x = (p2.x-p1.x) * x + p1.x;
        l2.y = (p2.y-p1.y) * y + p1.y;
        RenderInterface::line(l1, l2);
        l1 = l2;
    }
}