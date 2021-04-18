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
    if (nodeboard && nodeboard->requestContextMenu)
    {
        ImGui::OpenPopup("New Node");
        nodeboard->requestContextMenu = false;
    }

    constexpr std::array<const char*, (size_t)NodeName_Count> nameStr = {"Add", "Mult", "Float", "Sampler", "Oscillator", "Quantizer", "Mix", "Envelop", "Output"};
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
