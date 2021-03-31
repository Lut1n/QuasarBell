#ifndef QUASAR_BELL_NODE_CONTEXT_MENU_HPP
#define QUASAR_BELL_NODE_CONTEXT_MENU_HPP

#include "gui/Gui.hpp"

//--------------------------------------------------------------
class NodeContextMenu : public GuiComponent
{
public:

    enum NodeName
    {
        NodeName_Add = 0,
        NodeName_Float,

        NodeName_Count,

        NodeName_None
    };

    NodeContextMenu();
    
    void render() override;

    float x,y;
    NodeName which = NodeName_None;
};

#endif // QUASAR_BELL_NODE_CONTEXT_MENU_HPP
