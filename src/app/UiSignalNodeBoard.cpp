#include "UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

#include "Audio/WavExporter.hpp"

#include "App/SignalNode.hpp"

#include "ReaderWriter/FileIO.hpp"
#include "ReaderWriter/RwHelpers.hpp"
#include "ReaderWriter/SignalOperationsRW.hpp"

#include "SignalOperation/OperationType.hpp"
#include "SignalOperation/Operations.hpp"

void OperationConnections::fill(UiConnections* ui, const OperationCollection& coll)
{
    for (auto link : ui->links)
    {
        auto co = link.second;
        auto pin1 = co.first;
        auto pin2 = co.second;
        int id1 = (int)coll.getId(dynamic_cast<SignalNode*>(pin1->parentNode));
        int id2 = (int)coll.getId(dynamic_cast<SignalNode*>(pin2->parentNode));
        int idx1 = (int)pin1->parentNode->getIndex(pin1);
        int idx2 = (int)pin2->parentNode->getIndex(pin2);
        if (pin1->isInput)
            entries.push_back(Entry{id2,idx2,id1,idx1});
        else
            entries.push_back(Entry{id1,idx1,id2,idx2});
    }
}

//--------------------------------------------------------------
void UiSignalNodeBoard::update(float t)
{
    auto& app = AppInterface::get();

    if(!_ready)
    {
        nodeboard = std::make_unique<UiNodeBoard>(vec2(320, 30), vec2(1070, 520));
        uiConnections = nodeboard->connections.get();
        uiConnections->handler = this;
        UiSystem::instance()->add(nodeboard.get(), true, true);
        app.nodeboard = nodeboard.get();

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
    
    if(app.resetProject)
    {
        for(auto& op : operations.operations) nodeboard->rem(op.second.get());
        operations.operations.clear();
        app.resetProject = false;
    }

    if(app.fileInput.confirmed)
    {
        if(app.fileInput.request == UserFileInput::Load_Prj)
        {
            for(auto& op : operations.operations) nodeboard->rem(op.second.get());
            operations.operations.clear();
            OperationConnections connections;
            JsonValue root = loadJsonFile(app.fileInput.filepath);
            loadFrom(root, operations, connections);
            for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
            for(auto co : connections.entries)
            {
                uiConnections->createLink(operations.getOperation(co.src)->outputs[co.src_index].get(), operations.getOperation(co.dst)->inputs[co.dst_index].get());
            }
        }
        else if(app.fileInput.request == UserFileInput::Save_Prj)
        {
            OperationConnections connections;
            connections.fill(uiConnections, operations);
            JsonValue root;
            writeInfo(root);
            saveInto(root, operations, connections);
            saveJsonFile(app.fileInput.filepath, root);
        }
        app.fileInput.request = UserFileInput::Nothing;
        app.fileInput.confirmed = false;
    }
    if(app.waveInput.confirmed)
    {
        if(OutputOperation::defaultOutput == nullptr)
        {
            std::cout << "Error: You must set an audio output node as default" << std::endl;
        }
        else if(app.waveInput.request == UserFileInput::Export_Wav)
        {
            auto& output = OutputOperation::defaultOutput;
            std::unique_ptr<PcmDataBase> pcm;
            if (output->sampleBits == AudioSettings::Format_Mono8 || output->sampleBits == AudioSettings::Format_Stereo8)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
            if (output->sampleBits == AudioSettings::Format_Mono16 || output->sampleBits == AudioSettings::Format_Stereo16)
                pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
            OutputOperation::defaultOutput->generate(*pcm);
            WavExporter::exportAsWAV(app.waveInput.filepath, *pcm);
        }
        app.waveInput.request = UserFileInput::Nothing;
        app.waveInput.confirmed = false;
    }

    auto& col = operations.operations;
    for(auto it = col.begin(); it != col.end();)
    {
        if (it->second->toDelete)
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

    auto type = app.nodeToCreateType;
    if (type != qb::OperationType_None && type < qb::OperationType_Count)
    {
        std::unique_ptr<SignalNode> u;
        u.reset( Factory<SignalNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);
    }
    app.nodeToCreateType = qb::OperationType_None;
}

void UiSignalNodeBoard::onConnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;
    bool node1_is_src = !a->isInput;

    int node1_index = (int)node1->getIndex(a);
    int node2_index = (int)node2->getIndex(b);

    SignalOperation* op1 = dynamic_cast<SignalNode*>(node1)->getOperation();
    SignalOperation* op2 = dynamic_cast<SignalNode*>(node2)->getOperation();

    if(node1_is_src)
        SignalOperation::setConnection(op1, node1_index, op2, node2_index);
    else
        SignalOperation::setConnection(op2, node2_index, op1, node1_index);
}

void UiSignalNodeBoard::onDisconnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;
    bool node1_is_src = !a->isInput;

    if(node1_is_src && node2)
    {
        int node2_index = (int)node2->getIndex(b);
        auto* opnode = dynamic_cast<SignalNode*>(node2);
        if (opnode)
            SignalOperation::remConnection(opnode->getOperation(), node2_index);
    }
    if(!node1_is_src && node1)
    {
        int node1_index = (int)node1->getIndex(a);
        auto* opnode = dynamic_cast<SignalNode*>(node1);
        if (opnode)
            SignalOperation::remConnection(opnode->getOperation(), node1_index);
    }
}

//--------------------------------------------------------------
size_t OperationCollection::getId(SignalNode* operation) const
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
size_t OperationCollection::addOperation(std::unique_ptr<SignalNode>& operation)
{
    size_t id = getFreeId();
    operations[id] = std::move(operation);
    return id;
}

//--------------------------------------------------------------
void OperationCollection::setOperation(size_t id, std::unique_ptr<SignalNode>& operation)
{
    operations[id] = std::move(operation);
}

//--------------------------------------------------------------
SignalNode* OperationCollection::getOperation(size_t id)
{
    auto it = operations.find(id);
    if (it != operations.end()) return it->second.get();
    return nullptr;
}