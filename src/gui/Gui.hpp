#ifndef QUASAR_BELL_GUI_HPP
#define QUASAR_BELL_GUI_HPP

#include <list>
#include <vector>

// glad and glfw3
// #include "glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

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
    Synth,
    BasicEditor,
    NodalEditor
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
    
    void makeCurrent(GuiComponent* component);
    
    // double getTime() const;
    
    void open();
    void close();
    
    //bool shouldClose();
    void display();
    
    bool hasEvent();
    KeyEvent popEvent();
    
    static void key_callback(/*GLFWwindow* window, */int key, int scancode, int action, int mods);

private:
    void ioMenuItem(const char* text, const std::string& default_filepath, UserFileInput& fileinput, UserFileInput::Req req);

    //static void error_callback(int error, const char* description);

public:
    AppState appState;
    UserFileInput fileInput;
    UserFileInput waveInput;

private:
    AboutPanel _aboutPanel;
    //GLFWwindow* _window = nullptr;
    GuiComponent* _currentComponent = nullptr;
    
    static std::list<KeyEvent> s_keyEvents;
};

#endif // QUASAR_BELL_GUI_HPP
