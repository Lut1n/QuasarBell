#include "NodalEditor.hpp"

#include "gui/nodal/AddSignalNode.hpp"
#include "gui/nodal/FloatSignalNode.hpp"

//--------------------------------------------------------------
NodalEditorComponentGroup::NodalEditorComponentGroup()
{
    nodeProperties = new NodePropertiesEdit();
    nodeContextMenu = new NodeContextMenu();
    add(nodeProperties);
    add(nodeContextMenu);
}
//--------------------------------------------------------------
NodalEditorComponentGroup::~NodalEditorComponentGroup()
{
    delete nodeProperties;
    delete nodeContextMenu;
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
        nodes.resize(3);
        nodes[0] = std::make_unique<FloatSignalNode>(vec2(400, 50));
        nodes[1] = std::make_unique<FloatSignalNode>(vec2(410, 200));
        nodes[2] = std::make_unique<AddSignalNode>(vec2(740, 100));

        for(auto& n : nodes)
            UiSystem::instance()->add(n.get(), true, true);
            
        cons = std::make_unique<UiConnections>();
        cons->createLink(nodes[0]->outputs[0].get(), nodes[2]->inputs[1].get());
        cons->createLink(nodes[1]->outputs[0].get(), nodes[2]->inputs[0].get());
        
        UiSystem::instance()->add(cons.get(), true, true);
        _ready = true;
    }

    auto& gui = *(_app->gui);
    gui.makeCurrent(&_components);

    if (_components.nodeContextMenu->which != NodeContextMenu::NodeName_None)
    {
        auto name = _components.nodeContextMenu->which;
        switch(name)
        {
            case NodeContextMenu::NodeName_Add:
            {
                auto node = std::make_unique<AddSignalNode>(UiSystem::instance()->contextMenuPosition);
                UiSystem::instance()->add(node.get(), true, true);
                nodes.emplace_back(std::move(node));
                break;
            }
            case NodeContextMenu::NodeName_Float:
            {
                auto node = std::make_unique<FloatSignalNode>(UiSystem::instance()->contextMenuPosition);
                UiSystem::instance()->add(node.get(), true, true);
                nodes.emplace_back(std::move(node));
                break;
            }
        };
        _components.nodeContextMenu->which = NodeContextMenu::NodeName_None;
    }

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