#include "App/AppInterface.hpp"

#include "imgui.h"

#include "Audio/Signal.hpp"

#include "ImageOperation/ImageOperations.hpp"

namespace qb
{
    AppInterface* appInstance = nullptr;
}

//--------------------------------------------------------------
AppInterface::AppInterface()
    : fileInput("project path", ".json", "./save/my_project.json")
    , waveInput("wav path", ".wav", "./exported/output.wav")
    , tgaInput("tga path", ".tga", "./exported/output.tga")
{
    operationNames = qb::getOperationNames();
    imageOperationNames = qb::getImageOperationNames();
    geometryOperationNames = qb::getGeometryOperationNames();
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
            tgaInput.displayItem("As TGA", UserFileInput::Export_Tga);
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
    ImGui::Begin("Node properties", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    if(!UiNode::selected.empty())
        UiNode::selected.front()->displayAttributes();
    ImGui::End();

    
    if(_bigPreviewOpened)
    {
        if(ImGui::Begin("Bigger preview",&_bigPreviewOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            auto preview = dynamic_cast<TexturePreview*>(stickyOperation->getPreview());
            ImGui::Image(RenderInterface::getTargetResource((unsigned)preview->glTextureId),
                        ImVec2(512, 512), ImVec2(0,0), ImVec2(1,1), ImVec4(1.0f,1.0f,1.0f,1.0f), ImVec4(1.0f,1.0f,1.0f,0.5f));
        }
        ImGui::End();
    }
    else
    {
        stickyOperation = nullptr;
    }
    
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
        if (ImGui::BeginMenu("Geometry"))
        {
            for(size_t i=0; i<geometryOperationNames.size(); ++i)
            {
                if(ImGui::MenuItem(geometryOperationNames[i].c_str()))
                {
                    nodeToCreateCategory = NodeCategory_Geometry;
                    nodeToCreateType = i;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    fileInput.display();
    waveInput.display();
    tgaInput.display();
    _aboutPanel.display();
}

void AppInterface::openBigPreview(BaseOperationNode* op)
{
    stickyOperation = op;
    _bigPreviewOpened = true;
}

void AppInterface::closeBigPreview()
{
    stickyOperation = nullptr;
    _bigPreviewOpened=false;
}

AppInterface& AppInterface::get()
{
    return *qb::appInstance;
}
