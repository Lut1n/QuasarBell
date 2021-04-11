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
    connections = std::make_unique<UiConnections>();
    add(connections.get());
}

UiNodeBoard::~UiNodeBoard()
{
}

/*
void UiNode::onMove(const vec2& delta)
{
    if (UiNode::isSelected(this))
    {
        for(auto n : selected) n->position += delta;
    }
    else
    {
        UiFrame::onMove(delta);
    }
}
*/

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
