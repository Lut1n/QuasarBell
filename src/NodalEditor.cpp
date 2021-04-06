#include "NodalEditor.hpp"

#include "gui/nodal/AddSignalNode.hpp"
#include "gui/nodal/FloatSignalNode.hpp"
#include "gui/nodal/LinearSamplerNode.hpp"
#include "gui/nodal/DebuggerNode.hpp"
#include "gui/nodal/OscillatorNode.hpp"
#include "gui/nodal/QuantizerNode.hpp"


#include "io/FileIO.hpp"
#include "io/SignalRW.hpp"
#include "signal/WavExporter.hpp"
#include "io/SignalOperationsRW.hpp"

App* App::s_instance = nullptr;

void OperationConnections::fill(UiConnections* ui, const OperationCollection& coll)
{
    for (auto link : ui->links)
    {
        auto co = link.second;
        auto pin1 = co.first;
        auto pin2 = co.second;
        int id1 = coll.getId(dynamic_cast<SignalOperationNode*>(pin1->parentNode));
        int id2 = coll.getId(dynamic_cast<SignalOperationNode*>(pin2->parentNode));
        int idx1 = pin1->parentNode->getIndex(pin1);
        int idx2 = pin2->parentNode->getIndex(pin2);
        if (pin1->multipleConnections)
            entries.push_back(Entry{id1,idx1,id2,idx2});
        else
            entries.push_back(Entry{id2,idx2,id1,idx1});
    }
}

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
    App::s_instance = app;
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
        std::unique_ptr<SignalOperationNode> u = std::make_unique<FloatSignalNode>(vec2(400, 50));
        operations.setOperation(0, u);
        u = std::make_unique<FloatSignalNode>(vec2(410, 200));
        operations.setOperation(1, u);
        u = std::make_unique<AddSignalNode>(vec2(740, 100));
        operations.setOperation(2, u);
        u = std::make_unique<DebuggerNode>(vec2(1000, 200));
        operations.setOperation(3, u);

        UiSystem::instance()->add(operations.getOperation(0), true, true);
        UiSystem::instance()->add(operations.getOperation(1), true, true);
        UiSystem::instance()->add(operations.getOperation(2), true, true);
        UiSystem::instance()->add(operations.getOperation(3), true, true);
            
        cons = std::make_unique<UiConnections>();
        cons->handler = this;
        cons->createLink(operations.getOperation(0)->outputs[0].get(), operations.getOperation(2)->inputs[0].get());
        cons->createLink(operations.getOperation(1)->outputs[0].get(), operations.getOperation(2)->inputs[1].get());
        
        UiSystem::instance()->add(cons.get(), true, true);
        _ready = true;
    }

    auto& gui = *(_app->gui);
    gui.makeCurrent(&_components);

    
    if(gui.fileInput.confirmed)
    {
        if(gui.fileInput.request == UserFileInput::Load_Prj)
        {
            for(auto& op : operations.operations) UiSystem::instance()->rem(op.second.get());
            operations.operations.clear();
            OperationConnections connections;
            JsonValue root = loadJsonFile(gui.fileInput.filepath);
            loadFrom(root, operations, connections);
            for(auto& op : operations.operations) UiSystem::instance()->add(op.second.get(), true, true);
            for(auto co : connections.entries)
            {
                cons->createLink(operations.getOperation(co.src)->outputs[co.src_index].get(), operations.getOperation(co.dst)->inputs[co.dst_index].get());
            }
        }
        else if(gui.fileInput.request == UserFileInput::Save_Prj)
        {
            OperationConnections connections;
            connections.fill(cons.get(), operations);
            JsonValue root;
            writeInfo(root);
            saveInto(root, operations, connections);
            saveJsonFile(gui.fileInput.filepath, root);
        }
        gui.fileInput.request = UserFileInput::Nothing;
        gui.fileInput.confirmed = false;
    }
    if(gui.waveInput.confirmed)
    {
        if(gui.waveInput.request == UserFileInput::Export_Wav)
        {
            //WavExporter::exportAsWAV(gui.waveInput.filepath, generate());
        }
        gui.waveInput.request = UserFileInput::Nothing;
        gui.waveInput.confirmed = false;
    }

    auto& col = operations.operations;
    for(auto it = col.begin(); it != col.end();)
    {
        if (it->second.get() && it->second->toDelete)
        {
            UiSystem::instance()->rem(it->second.get());
            it = col.erase(it);
        }
        else if (!it->second)
        {
            it = col.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (_components.nodeContextMenu->which != NodeContextMenu::NodeName_None)
    {
        auto name = _components.nodeContextMenu->which;
        auto p = UiSystem::instance()->contextMenuPosition;

        std::unique_ptr<SignalOperationNode> u;

        switch(name)
        {
            case NodeContextMenu::NodeName_Add:
                u = std::make_unique<AddSignalNode>(p);
                break;
            case NodeContextMenu::NodeName_Mult:
                u = std::make_unique<MultSignalNode>(p);
                break;
            case NodeContextMenu::NodeName_Float:
                u = std::make_unique<FloatSignalNode>(p);
                break;
            case NodeContextMenu::NodeName_Debug:
                u = std::make_unique<DebuggerNode>(p);
                break;
            case NodeContextMenu::NodeName_LinearSampler:
                u = std::make_unique<LinearSamplerNode>(p);
                break;
            case NodeContextMenu::NodeName_Oscillator:
                u = std::make_unique<OscillatorNode>(p);
                break;
            case NodeContextMenu::NodeName_Quantizer:
                u = std::make_unique<QuantizerNode>(p);
                break;
        };

        if(u)
        {
            size_t id = operations.addOperation(u);
            UiSystem::instance()->add(operations.getOperation(id), true, true);
        }
        _components.nodeContextMenu->which = NodeContextMenu::NodeName_None;
    }
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

void NodalEditorWorkSpace::onConnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;
    bool node1_is_src = a->multipleConnections;

    int node1_index = node1->getIndex(a);
    int node2_index = node2->getIndex(b);

    SignalOperation* op1 = dynamic_cast<SignalOperationNode*>(node1)->getOperation();
    SignalOperation* op2 = dynamic_cast<SignalOperationNode*>(node2)->getOperation();

    if(node1_is_src)
        SignalOperation::setConnection(op1, node1_index, op2, node2_index);
    else
        SignalOperation::setConnection(op2, node2_index, op1, node1_index);
}

void NodalEditorWorkSpace::onDisconnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;
    bool node1_is_src = a->multipleConnections;



    if(node1_is_src && node2)
    {
        int node2_index = node2->getIndex(b);
        auto* opnode = dynamic_cast<SignalOperationNode*>(node2);
        if (opnode)
            SignalOperation::remConnection(/*nullptr, 0, */opnode->getOperation(), node2_index);
    }
    if(!node1_is_src && node1)
    {
        int node1_index = node1->getIndex(a);
        auto* opnode = dynamic_cast<SignalOperationNode*>(node1);
        if (opnode)
            SignalOperation::remConnection(/*nullptr, 0, */opnode->getOperation(), node1_index);
    }
    
}




//--------------------------------------------------------------
size_t OperationCollection::getId(SignalOperationNode* operation) const
{
    auto it = std::find_if(operations.begin(), operations.end(), [operation](auto& pair) {return operation == pair.second.get();});
    if (it == operations.end()) return 0; // todo: do not that
    return it->first;
}

//--------------------------------------------------------------
size_t OperationCollection::getFreeId() const
{
    size_t id = 0;
    while ( operations.find(id) != operations.end() ) ++id;
    return id;
}

//--------------------------------------------------------------
size_t OperationCollection::addOperation(std::unique_ptr<SignalOperationNode>& operation)
{
    size_t id = getFreeId();
    operations[id] = std::move(operation);
    return id;
}

//--------------------------------------------------------------
void OperationCollection::setOperation(size_t id, std::unique_ptr<SignalOperationNode>& operation)
{
    operations[id] = std::move(operation);
}

//--------------------------------------------------------------
SignalOperationNode* OperationCollection::getOperation(size_t id)
{
    auto it = operations.find(id);
    if (it != operations.end()) return it->second.get();
    return nullptr;
}