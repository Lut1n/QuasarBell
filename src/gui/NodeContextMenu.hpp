#ifndef QUASAR_BELL_NODE_CONTEXT_MENU_HPP
#define QUASAR_BELL_NODE_CONTEXT_MENU_HPP

#include "gui/Gui.hpp"

#include "UI/UiNodeBoard.h"

//--------------------------------------------------------------
class NodeContextMenu : public GuiComponent
{
public:

    enum NodeName
    {
        NodeName_Add = 0,
        NodeName_Mult,
        NodeName_Float,
        NodeName_LinearSampler,
        NodeName_Oscillator,
        NodeName_Quantizer,
        NodeName_Debug,

        NodeName_Count,

        NodeName_None
    };

    NodeContextMenu();
    
    void render() override;

    float x,y;
    NodeName which = NodeName_None;
    UiNodeBoard* nodeboard = nullptr;
};

#endif // QUASAR_BELL_NODE_CONTEXT_MENU_HPP
