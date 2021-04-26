#include "UI/UiNodeBoard.h"

// std::list<UiNode*> UiNode::selected;

/*bool UiNode::isSelected(UiNode* node)
{
    for (auto n : selected)
        if (n == node) return true;
    return false;
}*/

UiNodeBoard::UiNodeBoard(const vec2& position, const vec2& size)
    : UiFrame(position, size)
{
    unsigned boardcolor = 0x010810FF;
    color = boardcolor;
    colorOnOver = boardcolor;
    colorOnIdle = boardcolor;
    borderEnabled = false;
    connections = std::make_unique<UiConnections>();
    add(connections.get());
}

UiNodeBoard::~UiNodeBoard()
{
}

void UiNodeBoard::startMove(const vec2& mousePos)
{
    if (UiSystem::instance()->multiselectionEnabled())
    {
        areaSelection = true;
        selectRect.p0 = mousePos;
        return;
    }
    children->foreachElement([](UiElement* el){
        UiNode* node = dynamic_cast<UiNode*>(el);
        if(node) node->startMovePosition = node->position;
        });
}

void UiNodeBoard::endMove(const vec2& mousePos)
{
    auto swapf = [](float& f1, float& f2) { float t=f1; f1=f2; f2=t; };

    if (areaSelection)
    {
        if (selectRect.p1.x < selectRect.p0.x) swapf(selectRect.p1.x, selectRect.p0.x);
        if (selectRect.p1.y < selectRect.p0.y) swapf(selectRect.p1.y, selectRect.p0.y);

        UiNode::selected.clear();
        children->foreachElement([&](UiElement* el){
            UiNode* node = dynamic_cast<UiNode*>(el);
            if(node && selectRect.inside(node->computeBounds() + position)) UiNode::selected.push_back(node);
            });
    }
    areaSelection = false;
}

void UiNodeBoard::onMove(const vec2& delta)
{
    if (areaSelection)
    {
        selectRect.p1 = selectRect.p0 + delta;
        return;
    }
    if (UiSystem::instance()->multiselectionEnabled()) return;
    children->foreachElement([delta](UiElement* el){
        UiNode* node = dynamic_cast<UiNode*>(el);
        if(node) node->position = node->startMovePosition + delta;
        });
}

bool UiNodeBoard::onEvent(const UiEvent& event)
{
    bool ret = UiFrame::onEvent(event);

    while(nextRClicked())
    {
        requestContextMenu = true;
        contextMenuPosition = event.position - position;
    }
    
    while(nextClicked())
    {
        UiNode::selected.clear();
    }

    return ret;
}

void UiNodeBoard::draw()
{
    UiFrame::draw();

    if(areaSelection)
    {
        RenderInterface::setColor(0xFFFFFFFF);
        RenderInterface::setThickness(2.0f);
        RenderInterface::rect(selectRect.p0, selectRect.p1);
    }
}