#include "App.hpp"
#include "Synth.hpp"
#include "SFXBasicEditor.hpp"

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
    GuiRenderer gui;
    gui.open();
    App app;
    app.gui = &gui;
    app.sound = &sound;
    
    // workspace mode
    BasicEditorWorkSpace basicEditor;
    SynthWorkSpace synth;
    basicEditor.init(&app);
    synth.init(&app);
    ToolWorkSpace* toolPtr = &basicEditor;
    
    // main loop
    while(!gui.shouldClose())
    {
        while(gui.hasEvent())
        {
            KeyEvent event = gui.popEvent();
            toolPtr->onEvent(event);
        }
        
        toolPtr->update( gui.getTime() );
        scene.update();
        gui.display();
    
        if( gui.appState.toolMode == ETool::BasicEditor)
            toolPtr = &basicEditor;
        else if( gui.appState.toolMode == ETool::Synth)
            toolPtr = &synth;
    }
    gui.close();
    return 0;
}

//-----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    int ret = runQuasarBell(argc, argv);
    return ret;
}
