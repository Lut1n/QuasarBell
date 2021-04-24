#include "NodalEditor.hpp"

#include "gui/nodal/AddSignalNode.hpp"
#include "gui/nodal/FloatSignalNode.hpp"
#include "gui/nodal/CubicSamplerNode.hpp"
#include "gui/nodal/DebuggerNode.hpp"
#include "gui/nodal/OscillatorNode.hpp"
#include "gui/nodal/QuantizerNode.hpp"
#include "gui/nodal/MixNode.hpp"
#include "gui/nodal/EnvelopNode.hpp"


#include "io/FileIO.hpp"
#include "io/SignalRW.hpp"
#include "signal/WavExporter.hpp"
#include "io/SignalOperationsRW.hpp"

#include "signal/operations/OperationType.hpp"

#include "Core/Factory.h"

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
        nodeboard = std::make_unique<UiNodeBoard>(vec2(320, 30), vec2(1070, 520));
        uiConnections = nodeboard->connections.get();
        uiConnections->handler = this;
        UiSystem::instance()->add(nodeboard.get(), true, true);
        // UiSystem::instance()->add(cons.get(), true, true);
        _components.nodeContextMenu->nodeboard = nodeboard.get();

        if (std::filesystem::exists("./default.json"))
        {
            for(auto& op : operations.operations) nodeboard->rem(op.second.get());
            operations.operations.clear();
            OperationConnections connections;
            JsonValue root = loadJsonFile("./default.json");
            loadFrom(root, operations, connections);
            for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
            for(auto co : connections.entries)
            {
                uiConnections->createLink(operations.getOperation(co.src)->outputs[co.src_index].get(), operations.getOperation(co.dst)->inputs[co.dst_index].get());
            }
        }
        _ready = true;
    }

    auto& gui = *(_app->gui);
    gui.makeCurrent(&_components);

    
    if(gui.appState.resetProject)
    {
        for(auto& op : operations.operations) nodeboard->rem(op.second.get());
        operations.operations.clear();
        gui.appState.resetProject = false;
    }

    if(gui.fileInput.confirmed)
    {
        if(gui.fileInput.request == UserFileInput::Load_Prj)
        {
            for(auto& op : operations.operations) nodeboard->rem(op.second.get());
            operations.operations.clear();
            OperationConnections connections;
            JsonValue root = loadJsonFile(gui.fileInput.filepath);
            loadFrom(root, operations, connections);
            for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
            for(auto co : connections.entries)
            {
                uiConnections->createLink(operations.getOperation(co.src)->outputs[co.src_index].get(), operations.getOperation(co.dst)->inputs[co.dst_index].get());
            }
        }
        else if(gui.fileInput.request == UserFileInput::Save_Prj)
        {
            OperationConnections connections;
            connections.fill(uiConnections, operations);
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
        if(gui.waveInput.request == UserFileInput::Export_Wav && DebuggerNode::defaultOutput != nullptr)
        {
            auto& output = DebuggerNode::defaultOutput->debug;
            std::unique_ptr<PcmDataBase> pcm;
            if (output.sampleBits == AudioSettings::Format_Mono8 || output.sampleBits == AudioSettings::Format_Stereo8)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
            if (output.sampleBits == AudioSettings::Format_Mono16 || output.sampleBits == AudioSettings::Format_Stereo16)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
            DebuggerNode::defaultOutput->generate(*pcm);
            WavExporter::exportAsWAV(gui.waveInput.filepath, *pcm);
        }
        gui.waveInput.request = UserFileInput::Nothing;
        gui.waveInput.confirmed = false;
    }

    auto& col = operations.operations;
    for(auto it = col.begin(); it != col.end();)
    {
        if (it->second.get() && it->second->toDelete)
        {
            nodeboard->rem(it->second.get());
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

    auto type = _components.nodeContextMenu->which;
    if (type != qb::OperationType_None && type < qb::OperationType_Count)
    {
        std::unique_ptr<SignalOperationNode> u;
        u.reset( Factory<SignalOperationNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);
    }
    _components.nodeContextMenu->which = qb::OperationType_None;
}
//--------------------------------------------------------------
void NodalEditorWorkSpace::render()
{
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
            SignalOperation::remConnection(opnode->getOperation(), node2_index);
    }
    if(!node1_is_src && node1)
    {
        int node1_index = node1->getIndex(a);
        auto* opnode = dynamic_cast<SignalOperationNode*>(node1);
        if (opnode)
            SignalOperation::remConnection(opnode->getOperation(), node1_index);
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