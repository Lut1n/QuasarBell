#include "gui/NodeContextMenu.hpp"

#include "imgui.h"

#include "UI/UiSystem.h"

//--------------------------------------------------------------
NodeContextMenu::NodeContextMenu()
{
}
//--------------------------------------------------------------
void NodeContextMenu::render()
{
    if (UiSystem::instance()->requestContextMenu)
    {
        ImGui::OpenPopup("New Node");
        UiSystem::instance()->requestContextMenu = false;
    }

    constexpr std::array<const char*, NodeName_Count> nameStr = {"Add", "Float"};
    if (ImGui::BeginPopup("New Node"))
    {
        for(size_t i=0; i<NodeName_Count; ++i)
        {
            if(ImGui::MenuItem(nameStr[i]))
            {
                which = (NodeName)i;
            }
        }
        ImGui::EndPopup();
    }
}
