#ifndef QUASAR_BELL_GUI_HPP
#define QUASAR_BELL_GUI_HPP

#include <list>
#include <vector>

// glad and glfw3
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "signal/PcmData.hpp"
#include "gui/UserFileInput.hpp"

//--------------------------------------------------------------
#define FWD_CLASS(x) class x

//--------------------------------------------------------------
struct KeyEvent
{
    char key;
    bool pressed;
    double time;
};
//--------------------------------------------------------------
class GuiModule
{
public:
    virtual ~GuiModule() = default;
    virtual void render() = 0;

public:
    bool isEnabled = false;
    bool hasChanged = false;
};
//--------------------------------------------------------------
class ModuleGroup
{
public:
    virtual ~ModuleGroup() = default;
    
    void add(GuiModule* module) {modules.push_back(module);}
    int count() const {return modules.size();}
    GuiModule* get(int i){return modules[i];}

public:
    std::vector<GuiModule*> modules;
};
//--------------------------------------------------------------
enum class ETool
{
    Synth,
    BasicEditor
};
//--------------------------------------------------------------
struct AppState
{
    ETool toolMode = ETool::BasicEditor;
};

//--------------------------------------------------------------
FWD_CLASS(FrequencyEdit);
FWD_CLASS(AmplitudeEdit);
FWD_CLASS(WaveEdit);
FWD_CLASS(PcmPlayer);

//--------------------------------------------------------------
class GuiRenderer
{
public:
    GuiRenderer();
    ~GuiRenderer() = default;
    
    void setModuleGroup(ModuleGroup* group);
    
    double getTime() const;
    
    void open();
    void close();
    
    bool shouldClose();
    void display();
    
    bool hasEvent();
    KeyEvent popEvent();
    
private:
    void ioMenuItem(const char* text, const std::string& default_filepath, UserFileInput& fileinput, UserFileInput::Req req);

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
    AppState appState;
    UserFileInput fileInput;
    UserFileInput waveInput;

private:
    AboutPanel _aboutPanel;
    GLFWwindow* _window = nullptr;
    ModuleGroup* _modules = nullptr;
    
    static std::list<KeyEvent> s_keyEvents;
};

#endif // QUASAR_BELL_GUI_HPP
