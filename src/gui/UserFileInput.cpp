#include "UserFileInput.hpp"

#include "imgui.h"

//--------------------------------------------------------------
UserFileInput::UserFileInput(const std::string& title, const std::string& ext)
    : _title(title)
    , _ext(ext)
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
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        _explorer.setCurrentPath(filepath);
        {
            const size_t last_slash_idx = filepath.find_last_of("\\/");
            if (std::string::npos != last_slash_idx)
                filepath.erase(0, last_slash_idx + 1);
        }
        strcpy(_editBuffer, filepath.c_str());
        requestOpen = false;
    }
    
    if(!ImGui::BeginPopupModal(_title.c_str()))
        return;

    if (ImGui::ListBoxHeader("Explorer", ImVec2(0, -100)))
    {
        if(ImGui::Selectable("##< parent", false))
        {
            _explorer.goToParentDir();
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.7, 0.7, 1, 1), "< parent");
        ImGui::Separator();
        size_t count = _explorer.getItemCount();
        for(size_t i=0; i<count; ++i)
        {
            auto info = _explorer.getItem(i);
            if (info.isRegular && info.ext == _ext)
            {
                if(ImGui::Selectable(info.name.c_str(), info.name == filepath))
                {
                    filepath = info.name;
                    strcpy(_editBuffer, filepath.c_str());
                }
            }
            else if (info.isDir)
            {
                std::string folder = std::string("> ") + info.name;
                std::string folder_sld = std::string("##") + folder;
                if(ImGui::Selectable(folder_sld.c_str(), false))
                {
                    _explorer.goToChildDir(i);
                }
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.7, 0.7, 1, 1), folder.c_str());
            }
        }
        ImGui::ListBoxFooter();
    }
    ImGui::InputText("path", _editBuffer, sizeof(_editBuffer));
    ImGui::Columns(2);
    if(ImGui::Button("Cancel")) close();
    ImGui::NextColumn();
    if(ImGui::Button("Confirm"))
    {
        filepath = _editBuffer;
        filepath = _explorer.buildPath(filepath, _ext);
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