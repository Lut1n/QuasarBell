#include "App/AppInterface.hpp"

#include "imgui.h"

#include "Audio/Signal.hpp"

namespace qb
{
    AppInterface* appInstance = nullptr;
}

//--------------------------------------------------------------
AppInterface::AppInterface()
    : fileInput("project path", ".json", "./save/my_project.json")
    , waveInput("wav path", ".wav", "./exported/output.wav")
{
    operationNames = qb::getOperationNames();
    imageOperationNames = qb::getImageOperationNames();
    qb::appInstance = this;
}

//--------------------------------------------------------------
void AppInterface::display()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Project"))
        {
            if( ImGui::MenuItem("New") ) resetProject = true;
            fileInput.displayItem("Load", UserFileInput::Load_Prj);
            fileInput.displayItem("Save", UserFileInput::Save_Prj);
            ImGui::Separator();
            if( ImGui::MenuItem("Close")) {exit(0);}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Export"))
        {
            waveInput.displayItem("As WAV", UserFileInput::Export_Wav);
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("About"))
        {
            _aboutPanel.open();
        }
        ImGui::EndMainMenuBar();
    }
    
    // property edit panel
    ImGui::SetNextWindowPos(ImVec2(6, 25), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 530), ImGuiCond_FirstUseEver);
    ImGui::Begin("Node properties", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    if(!UiNode::selected.empty())
        UiNode::selected.front()->displayProperties();
    ImGui::End();
    
    // context menu
    if (nodeboard && nodeboard->requestContextMenu)
    {
        ImGui::OpenPopup("New Node");
        nodeboard->requestContextMenu = false;
        nodeToCreateCategory = NodeCategory_None;
    }
    if (ImGui::BeginPopup("New Node"))
    {
        ImGui::Text("New Node");
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::BeginMenu("Audio"))
        {
            for(size_t i=0; i<operationNames.size(); ++i)
            {
                if(ImGui::MenuItem(operationNames[i].c_str()))
                {
                    nodeToCreateCategory = NodeCategory_Signal;
                    nodeToCreateType = i;
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Image"))
        {
            for(size_t i=0; i<imageOperationNames.size(); ++i)
            {
                if(ImGui::MenuItem(imageOperationNames[i].c_str()))
                {
                    nodeToCreateCategory = NodeCategory_Image;
                    nodeToCreateType = i;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    fileInput.display();
    waveInput.display();
    _aboutPanel.display();
}

AppInterface& AppInterface::get()
{
    return *qb::appInstance;
}