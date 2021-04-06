#include "gui/NodeContextMenu.hpp"

#include "imgui.h"

#include "UI/UiSystem.h"

#include <array>

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

    constexpr std::array<const char*, (size_t)NodeName_Count> nameStr = {"Add", "Mult", "Float", "LinearSampler", "Oscillator", "Quantizer", "DebugOutput"};
    if (ImGui::BeginPopup("New Node"))
    {
        ImGui::Text("New Node");
        ImGui::Separator();
        ImGui::Spacing();
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
