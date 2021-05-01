#include "gui/Gui.hpp"

#include "imgui.h"
#include "gui/imgui_impl_glfw.h"
#include "gui/imgui_impl_opengl3.h"

#include "signal/Signal.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

//--------------------------------------------------------------
std::list<KeyEvent> GuiRenderer::s_keyEvents;

//--------------------------------------------------------------
void GuiRenderer::ioMenuItem(const char* text, const std::string& default_filepath, UserFileInput& fileinput, UserFileInput::Req req)
{
    if(ImGui::MenuItem(text))
    {
        if(fileinput.filepath.empty())
            fileinput.filepath = default_filepath;
        fileinput.request = req;
        fileinput.open();
    }
}
//--------------------------------------------------------------
/*void GuiRenderer::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}*/
//--------------------------------------------------------------
void GuiRenderer::key_callback(/*GLFWwindow* window, */int key, int scancode, int action, int mods)
{
    /*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    */
    if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_RELEASE))
    {
        char k = (key-GLFW_KEY_A) + 'a';
        KeyEvent kevent;
        kevent.key = k;
        kevent.pressed = action == GLFW_PRESS;
        kevent.time = glfwGetTime();
        s_keyEvents.push_back(kevent);
    }        
}
//--------------------------------------------------------------
void GuiComponentGroup::add(GuiComponent* component)
{
    components.push_back(component);
}
//--------------------------------------------------------------
int GuiComponentGroup::count() const
{
    return (int)components.size();
}
//--------------------------------------------------------------
GuiComponent* GuiComponentGroup::get(int i)
{
    return components[i];
}
//--------------------------------------------------------------
void GuiComponentGroup::render()
{
    for (auto& c : components)
        c->render();
}
//--------------------------------------------------------------
GuiRenderer::GuiRenderer()
    : fileInput("project path", ".json")
    , waveInput("wav path", ".wav")
{
}
//--------------------------------------------------------------
void GuiRenderer::makeCurrent(GuiComponent* component)
{
    _currentComponent = component;
}
//--------------------------------------------------------------
void GuiRenderer::open()
{
    /*glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    _window = glfwCreateWindow(1400, 560, "QuasarBell", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetKeyCallback(_window, key_callback);
    
    glfwMakeContextCurrent(_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);*/
    
    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);*/
}
//--------------------------------------------------------------
void GuiRenderer::close()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    //glfwDestroyWindow(_window);
    //glfwTerminate();
}
//--------------------------------------------------------------
/*double GuiRenderer::getTime() const
{
    return glfwGetTime();
}*/
//--------------------------------------------------------------
/*bool GuiRenderer::shouldClose()
{
    return glfwWindowShouldClose(_window);
}*/
//--------------------------------------------------------------
void GuiRenderer::display()
{
    /*int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);*/
    
    /*ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();*/
    
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("Project"))
        {
            if( ImGui::MenuItem("New") ) appState.resetProject = true;
            ioMenuItem("Load", "./save/my_project.json", fileInput, UserFileInput::Load_Prj);
            ioMenuItem("Save", "./save/my_project.json", fileInput, UserFileInput::Save_Prj);
            ImGui::Separator();
            if( ImGui::MenuItem("Close")) {exit(0);}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Tools"))
        {
            if( ImGui::MenuItem("SFX Edition") ) appState.toolMode = ETool::BasicEditor;
            if( ImGui::MenuItem("Synth") ) appState.toolMode = ETool::Synth;
            if( ImGui::MenuItem("Nodal Editor") ) appState.toolMode = ETool::NodalEditor;
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Export"))
        {
            ioMenuItem("As WAV", "./exported/output.wav", waveInput, UserFileInput::Export_Wav);
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("About"))
        {
            _aboutPanel.open();
        }
        ImGui::EndMainMenuBar();
    }
    if(_currentComponent)
        _currentComponent->render();
    
    fileInput.display();
    waveInput.display();
    _aboutPanel.display();
    
    /*ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

    /*glfwSwapBuffers(_window);
    glfwPollEvents();*/
}
//--------------------------------------------------------------
bool GuiRenderer::hasEvent()
{
    return s_keyEvents.size() > 0;
}
//--------------------------------------------------------------
KeyEvent GuiRenderer::popEvent()
{
    KeyEvent nextEvent = s_keyEvents.front();
    s_keyEvents.pop_front();
    return nextEvent;
}
