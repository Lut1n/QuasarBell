#ifndef QUASAR_BELL_NODE_CONTEXT_MENU_HPP
#define QUASAR_BELL_NODE_CONTEXT_MENU_HPP

#include "gui/Gui.hpp"

#include "signal/operations/OperationType.hpp"
#include "UI/UiNodeBoard.h"

//--------------------------------------------------------------
class NodeContextMenu : public GuiComponent
{
public:
    NodeContextMenu();
    
    void render() override;

    float x,y;
    qb::OperationType which = qb::OperationType_None;
    UiNodeBoard* nodeboard = nullptr;
    std::vector<std::string> operationNames;
};

#endif // QUASAR_BELL_NODE_CONTEXT_MENU_HPP
