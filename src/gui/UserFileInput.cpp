#include "UserFileInput.hpp"

#include "imgui.h"

//--------------------------------------------------------------
UserFileInput::UserFileInput(const std::string& title)
    : _title(title)
{
}
//--------------------------------------------------------------
UserFileInput::~UserFileInput()
{
}
//--------------------------------------------------------------
void UserFileInput::open()
{
    requestOpen = true;
}
//--------------------------------------------------------------
void UserFileInput::close()
{
    ImGui::CloseCurrentPopup();
}
//--------------------------------------------------------------
void UserFileInput::display()
{
    if(requestOpen)
    {
        ImGui::OpenPopup(_title.c_str());
        ImGui::SetNextWindowSize(ImVec2(400, 100));
        strcpy(_editBuffer, filepath.c_str());
        requestOpen = false;
    }
    
    if(!ImGui::BeginPopupModal(_title.c_str()))
        return;

    ImGui::InputText("path", _editBuffer, sizeof(_editBuffer));
    ImGui::Columns(2);
    if(ImGui::Button("Cancel")) close();
    ImGui::NextColumn();
    if(ImGui::Button("Confirm"))
    {
        filepath = _editBuffer;
        confirmed = true;
        close();
    }
    ImGui::NextColumn();
    ImGui::Columns(1);
    
    ImGui::EndPopup();
}
//--------------------------------------------------------------
AboutPanel::AboutPanel()
{
}
//--------------------------------------------------------------
void AboutPanel::open()
{
    requestOpen = true;
}
//--------------------------------------------------------------
void AboutPanel::close()
{
    ImGui::CloseCurrentPopup();
}
//--------------------------------------------------------------
void AboutPanel::display()
{
    if(requestOpen)
    {
        ImGui::OpenPopup("About");
        ImGui::SetNextWindowSize(ImVec2(400, 150));
        requestOpen = false;
    }
    
    if(!ImGui::BeginPopupModal("About"))
        return;

    ImGui::Text("QuasarBell");
    ImGui::Text("version 1.0");
    ImGui::Text("2021 - Mathieu Boulet");
    ImGui::Text("sources: https://www.github.com/Lut1n/QuasarBell");
    if(ImGui::Button("Okay"))
        close();
    
    ImGui::EndPopup();

}