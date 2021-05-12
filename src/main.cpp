#include "core/Factory.h"
#include "app/App.hpp"
#include "app/NodalEditor.hpp"

#include "graphics/RenderInterface.h"
#include "font/GlyphRenderer.h"
#include "ui/UiSystem.h"

//--------------------------------------------------------------
int runQuasarBell(int argc, char* argv[])
{
    // audio renderer
    Scene scene;
    MicroNode micro;
    SoundNode sound(true);
    scene.add(&micro);
    scene.add(&sound);    

    // user interface
    unsigned win = RenderInterface::createTarget(1400, 560, true, "QuasarBell");
    UiSystem::instance()->initialize();
    // auto& gr = UiSystem::instance()->glyphRenderer;
    GuiRenderer gui;
    gui.open();
    App app;
    app.gui = &gui;
    app.sound = &sound;
    
    // workspace mode
    NodalEditorWorkSpace nodalEditor;
    nodalEditor.init(&app);
    ToolWorkSpace* toolPtr = &nodalEditor;
    
    // main loop
    RenderInterface::setTarget(win);
    while(RenderInterface::begin())
    {
        RenderInterface::setTarget(win);
        RenderInterface::clear(0x1e1e1eFF);
        
        RenderInterface::updateTime();
        UiSystem::instance()->draw();

        while(gui.hasEvent())
        {
            KeyEvent event = gui.popEvent();
            toolPtr->onEvent(event);
        }
        
        toolPtr->update( (float)RenderInterface::getTime() );
        scene.update();
        gui.display();
    
        if( gui.appState.toolMode == ETool::NodalEditor)
            toolPtr = &nodalEditor;
        UiSystem::instance()->setActivated(gui.appState.toolMode == ETool::NodalEditor);
        RenderInterface::end();
    }
    gui.close();
    return 0;
}

//-----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    static BaseFactory default_factory;
    int ret = runQuasarBell(argc, argv);
    return ret;
}
