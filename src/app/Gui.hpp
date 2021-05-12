#ifndef QUASAR_BELL_GUI_HPP
#define QUASAR_BELL_GUI_HPP

#include <list>
#include <vector>

// glad and glfw3
// #include "glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "Audio/PcmData.hpp"
#include "App/UserFileInput.hpp"

//--------------------------------------------------------------
struct KeyEvent
{
    char key;
    bool pressed;
    double time;
};
//--------------------------------------------------------------
class GuiComponent
{
public:
    virtual ~GuiComponent() = default;
    virtual void render() = 0;

public:
    bool isEnabled = false;
    bool hasChanged = false;
};
//--------------------------------------------------------------
class GuiComponentGroup : public GuiComponent
{
public:
    void add(GuiComponent* component);
    int count() const;
    GuiComponent* get(int i);

    void render() override;

public:
    std::vector<GuiComponent*> components;
};
//--------------------------------------------------------------
enum class ETool
{
    //Synth,
    //BasicEditor,
    NodalEditor
};
//--------------------------------------------------------------
struct AppState
{
    ETool toolMode = ETool::NodalEditor;
    bool resetProject = false;
};

//--------------------------------------------------------------
class GuiRenderer
{
public:
    GuiRenderer();
    ~GuiRenderer() = default;
    
    void makeCurrent(GuiComponent* component);
    
    void open();
    void close();
    
    void display();
    
    bool hasEvent();
    KeyEvent popEvent();
    
    static void key_callback(int key, int scancode, int action, int mods);

private:
    void ioMenuItem(const char* text, const std::string& default_filepath, UserFileInput& fileinput, UserFileInput::Req req);

public:
    AppState appState;
    UserFileInput fileInput;
    UserFileInput waveInput;

private:
    AboutPanel _aboutPanel;
    GuiComponent* _currentComponent = nullptr;
    
    static std::list<KeyEvent> s_keyEvents;
};

#endif // QUASAR_BELL_GUI_HPP
