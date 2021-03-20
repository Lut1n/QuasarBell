#include "NodalEditor.hpp"


//--------------------------------------------------------------
NodalEditorComponentGroup::NodalEditorComponentGroup()
{
}
//--------------------------------------------------------------
NodalEditorComponentGroup::~NodalEditorComponentGroup()
{
}

//--------------------------------------------------------------
void NodalEditorWorkSpace::init(App* app)
{
    _app = app;
}
//--------------------------------------------------------------
void NodalEditorWorkSpace::onEvent(const KeyEvent& event)
{
}
//--------------------------------------------------------------
void NodalEditorWorkSpace::update(double t)
{
    if(!_ready)
    {
        nodes.resize(5);
        nodes[0] = std::make_unique<UiNode>("time", vec2(500, 50), vec2(100, 100));
        nodes[0]->addPin(0, "seconds", true);

        nodes[1] = std::make_unique<UiNode>("freq-edit", vec2(530, 270), vec2(100, 100));
        nodes[1]->addPin(0, "time", false);
        nodes[1]->addPin(0, "freq", true);
        
        nodes[2] = std::make_unique<UiNode>("ampl-edit", vec2(540, 290), vec2(100, 100));
        nodes[2]->addPin(0, "time", false);
        nodes[2]->addPin(0, "ampl", true);

        nodes[3] = std::make_unique<UiNode>("signal", vec2(520, 200), vec2(100, 100));
        nodes[3]->addPin(0, "frequency", false);
        nodes[3]->addPin(1, "amplitude", false);
        nodes[3]->addPin(0, "pcm", true);

        nodes[4] = std::make_unique<UiNode>("output", vec2(540, 440), vec2(100, 100));
        nodes[4]->addPin(0, "pcm", false);

        for(auto& n : nodes)
            UiSystem::instance()->add(n.get(), true, true);
            
        cons = std::make_unique<UiConnections>();
        UiSystem::instance()->add(cons.get(), true, true);
        
        _ready = true;
    }

    auto& gui = *(_app->gui);
    gui.makeCurrent(&_components);

    // while(node1->nextClicked()) selected->text = node1->title->text;
}
//--------------------------------------------------------------
void NodalEditorWorkSpace::render()
{
}
//--------------------------------------------------------------
PcmData NodalEditorWorkSpace::generate()
{
    PcmData output;
    return output;
}