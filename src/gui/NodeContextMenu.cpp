#include "gui/NodeContextMenu.hpp"

#include "imgui.h"

#include "UI/UiSystem.h"

#include <array>

//--------------------------------------------------------------
NodeContextMenu::NodeContextMenu()
{
    operationNames = qb::getOperationNames();
}
//--------------------------------------------------------------
void NodeContextMenu::render()
{
    if (nodeboard && nodeboard->requestContextMenu)
    {
        ImGui::OpenPopup("New Node");
        nodeboard->requestContextMenu = false;
    }
    if (ImGui::BeginPopup("New Node"))
    {
        ImGui::Text("New Node");
        ImGui::Separator();
        ImGui::Spacing();
        for(size_t i=0; i<operationNames.size(); ++i)
        {
            if(ImGui::MenuItem(operationNames[i].c_str()))
            {
                which = (qb::OperationType)i;
            }
        }
        ImGui::EndPopup();
    }
}
