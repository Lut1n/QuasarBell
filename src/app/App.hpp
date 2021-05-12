#ifndef QUASAR_BELL_APP_H
#define QUASAR_BELL_APP_H

#include "Audio/AudioRenderer.hpp"
#include "App/Gui.hpp"

//--------------------------------------------------------------
class App
{
public:
    GuiRenderer* gui;
    SoundNode* sound;

    static App* s_instance;
};

//--------------------------------------------------------------
class ToolWorkSpace
{
public:
    virtual void init(App* app) = 0;
    virtual void update(float t) = 0;
    virtual void onEvent(const KeyEvent& event) = 0;
    virtual void render() = 0;
};

#endif // QUASAR_BELL_APP_H
