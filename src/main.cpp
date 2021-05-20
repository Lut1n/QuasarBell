#include "App/AppInterface.hpp"
#include "App/UiSignalNodeBoard.hpp"
#include "App/UiImageNodeBoard.hpp"

#include "Graphics/RenderInterface.h"
#include "Font/GlyphRenderer.h"
#include "Ui/UiSystem.h"
#include "Audio/AudioRenderer.hpp"

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
    AppInterface app;
    
    // workspace mode
    UiSignalNodeBoard nodalEditor;
    
    // main loop
    RenderInterface::setTarget(win);
    while(RenderInterface::begin())
    {
        nodalEditor.initializePreviews();
        nodalEditor.updatePreviews();

        RenderInterface::setTarget(win);
        RenderInterface::clear(0x1e1e1eFF);
        
        RenderInterface::updateTime();
        UiSystem::instance()->draw();
        
        nodalEditor.update( (float)RenderInterface::getTime() );
        scene.update();
        app.display();

        UiSystem::instance()->setActivated(true);
        RenderInterface::end();
    }
    RenderInterface::shutdown();
    return 0;
}

//-----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    int ret = runQuasarBell(argc, argv);
    return ret;
}
