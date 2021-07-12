#include "UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

#include "Audio/WavExporter.hpp"

#include "App/SignalNode.hpp"
#include "App/ImageNode.hpp"
#include "App/SdfNode.hpp"

#include "ReaderWriter/FileIO.hpp"
#include "ReaderWriter/RwHelpers.hpp"
#include "ReaderWriter/SignalOperationsRW.hpp"

#include "SignalOperation/OperationType.hpp"
#include "SignalOperation/Operations.hpp"

#include "ImageOperation/ImageOperationType.hpp"
#include "ImageOperation/ImageOperation.hpp"

#include "SdfOperation/SdfOperationType.hpp"
#include "SdfOperation/SdfOperation.hpp"

#include "ImageOperation/HighResOutput.hpp"

void OperationConnections::fill(UiConnections* ui, const OperationCollection& coll, UiPin::Type pinType)
{
    for (auto link : ui->links)
    {
        auto co = link.second;
        auto pin1 = co.first;
        auto pin2 = co.second;
        if (pin1->type != pinType || pin2->type != pinType) continue;
        auto node1 = dynamic_cast<BaseOperationNode*>(pin1->parentNode);
        auto node2 = dynamic_cast<BaseOperationNode*>(pin2->parentNode);
        if(!node1 || !node2) continue;
        int id1 = (int)coll.getId(node1);
        int id2 = (int)coll.getId(node2);
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
        nodeboard = std::make_unique<UiNodeBoard>(vec2(0, 0), RenderInterface::getCurrentTargetSize());
        uiConnections = nodeboard->connections.get();
        uiConnections->handler = this;
        UiSystem::instance()->add(nodeboard.get(), true, true);
        app.nodeboard = nodeboard.get();

        if (std::filesystem::exists("./default.json"))
        {
            load("./default.json");
        }
        _ready = true;
    }

    nodeboard->size = RenderInterface::getCurrentTargetSize();
    
    if(app.resetProject)
    {
        cleanup();
        app.resetProject = false;
    }

    if(app.fileInput.confirmed)
    {
        if(app.fileInput.request == UserFileInput::Load_Prj)
            load(app.fileInput.filepath);
        else if(app.fileInput.request == UserFileInput::Save_Prj)
            save(app.fileInput.filepath);
        app.fileInput.request = UserFileInput::Nothing;
        app.fileInput.confirmed = false;
    }
    if(app.waveInput.confirmed)
    {
        if(OutputOperation::defaultOutput == nullptr)
            std::cout << "Error: You must set an audio output node as default" << std::endl;
        else if(app.waveInput.request == UserFileInput::Export_Wav)
            exportWav(app.waveInput.filepath, *OutputOperation::defaultOutput);
        app.waveInput.request = UserFileInput::Nothing;
        app.waveInput.confirmed = false;
    }
    if(app.tgaInput.confirmed)
    {
        if(HighResOutput::defaultOutput == nullptr)
            std::cout << "Error: You must set an highres output node as default" << std::endl;
        else if(app.tgaInput.request == UserFileInput::Export_Tga)
            exportTga(app.tgaInput.filepath, *HighResOutput::defaultOutput);
        app.tgaInput.request = UserFileInput::Nothing;
        app.tgaInput.confirmed = false;
    }

    auto& col = operations.operations;
    for(auto it = col.begin(); it != col.end();)
    {
        if (it->second->toDelete)
        {
            it->second->disconnectAllPins();
            nodeboard->rem(it->second.get());
            it = col.erase(it);
        }
        else
        {
            ++it;
        }
    }

    auto category = app.nodeToCreateCategory;
    auto type = app.nodeToCreateType;
    if(category == NodeCategory_Signal && type != qb::OperationType_None && type < qb::OperationType_Count)
    {
        std::unique_ptr<BaseOperationNode> u;
        u.reset( Factory<SignalNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);    
        nodeboard->rem(uiConnections);
        nodeboard->add(uiConnections);
    }
    if(category == NodeCategory_Image && type != qb::ImageOperationType_None && type < qb::ImageOperationType_Count)
    {
        std::unique_ptr<BaseOperationNode> u;
        u.reset( Factory<ImageNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);
        nodeboard->rem(uiConnections);
        nodeboard->add(uiConnections);
    }
    if(category == NodeCategory_Geometry && type != qb::SdfOperationType_None && type < qb::SdfOperationType_Count)
    {
        std::unique_ptr<BaseOperationNode> u;
        u.reset( Factory<GeometryNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);
        nodeboard->rem(uiConnections);
        nodeboard->add(uiConnections);
    }
    app.nodeToCreateCategory = NodeCategory_None;
}
//--------------------------------------------------------------
void UiSignalNodeBoard::cleanup()
{
    for(auto& op : operations.operations)
    {
        op.second->disconnectAllPins();
        nodeboard->rem(op.second.get());
    }
    operations.operations.clear();
}
//--------------------------------------------------------------
void UiSignalNodeBoard::load(const std::string& path)
{
    cleanup();
    OperationConnections connections;
    JsonValue root = loadJsonFile(path);
    loadFrom(root, operations, connections, "sfx-nodal", UiPin::TYPE_FLOAT1,
            [](const std::string& type){return Factory<SignalNode>::create(qb::getOperationType(type));});
    loadFrom(root, operations, connections, "texture-nodal", UiPin::TYPE_FLOAT2,
            [](const std::string& type){return Factory<ImageNode>::create(qb::getImageOperationType(type));});

    operations.centerNodes(Rect::fromPosAndSize(vec2(0.0f,0.0f),nodeboard->size));
    for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
    for(auto co : connections.entries)
    {
        auto pin1 = operations.getOperation(co.src)->outputs[co.src_index].get();
        auto pin2 = operations.getOperation(co.dst)->inputs[co.dst_index].get();
        uiConnections->createLink(pin1, pin2);
    }
    nodeboard->rem(uiConnections);
    nodeboard->add(uiConnections);
}
//--------------------------------------------------------------
void UiSignalNodeBoard::save(const std::string& path)
{
    JsonValue root;
    writeInfo(root);
    OperationConnections connections, iconnections;
    connections.fill(uiConnections, operations, UiPin::TYPE_FLOAT1);
    saveInto(root, operations, connections, "sfx-nodal", UiPin::TYPE_FLOAT1,
            [](BaseOperationNode* node){return qb::getOperationName(static_cast<qb::OperationType>(node->nodeTypeId()));});
    iconnections.fill(uiConnections, operations, UiPin::TYPE_FLOAT2);
    saveInto(root, operations, iconnections, "texture-nodal", UiPin::TYPE_FLOAT2,
            [](BaseOperationNode* node){return qb::getImageOperationName(static_cast<qb::ImageOperationType>(node->nodeTypeId()));});
    saveJsonFile(path, root);
}
//--------------------------------------------------------------
void UiSignalNodeBoard::exportWav(const std::string& path, OutputOperation& outputNode)
{
    std::unique_ptr<PcmDataBase> pcm;
    if (outputNode.sampleBits == AudioSettings::Format_Mono8 || outputNode.sampleBits == AudioSettings::Format_Stereo8)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono8>>();
    if (outputNode.sampleBits == AudioSettings::Format_Mono16 || outputNode.sampleBits == AudioSettings::Format_Stereo16)
        pcm = std::make_unique<PcmData<AudioSettings::Format_Mono16>>();
    outputNode.generate(*pcm);
    WavExporter::exportAsWAV(path, *pcm);
}
//--------------------------------------------------------------
void UiSignalNodeBoard::exportTga(const std::string& path, HighResOutput& outputNode)
{
    if(auto frame = outputNode.preview.renderFrame.get())
    {
        qb::ImageData image = RenderInterface::downloadTargetImage((unsigned)frame->glResource);
        qb::exportTGA(path, image);
    }
}
//--------------------------------------------------------------
void UiSignalNodeBoard::updatePreviews()
{
    if(RenderInterface::getTime() - _lastUpdateTime < 0.0) return;
    _lastUpdateTime = RenderInterface::getTime();

    auto& col = operations.operations;
    for (auto& op : col) op.second->updatePreview();
}
//--------------------------------------------------------------
void UiSignalNodeBoard::onConnect(UiPin* a, UiPin* b)
{
    auto* node1 = dynamic_cast<BaseOperationNode*>(a->parentNode);
    auto* node2 = dynamic_cast<BaseOperationNode*>(b->parentNode);
    if(node1->getOperation()->getPinType() == node2->getOperation()->getPinType())
    {
        bool node1_is_src = !a->isInput;

        int node1_index = (int)node1->getIndex(a);
        int node2_index = (int)node2->getIndex(b);

        BaseOperation* op1 =node1->getOperation();
        BaseOperation* op2 = node2->getOperation();

        if(node1_is_src)
            BaseOperation::setConnection(op1, node1_index, op2, node2_index);
        else
            BaseOperation::setConnection(op2, node2_index, op1, node1_index);
    }
    else
    {
        std::cout << "Error: nodes have incompatible type" << std::endl;
    }
}
//--------------------------------------------------------------
void UiSignalNodeBoard::onDisconnect(UiPin* a, UiPin* b)
{
    auto* node1 = dynamic_cast<BaseOperationNode*>(a->parentNode);
    auto* node2 = dynamic_cast<BaseOperationNode*>(b->parentNode);

    if(node1->getOperation()->getPinType() == node2->getOperation()->getPinType())
    {
        bool node1_is_src = !a->isInput;

        if(node1_is_src)
        {
            int node2_index = (int)node2->getIndex(b);
            BaseOperation::remConnection(node2->getOperation(), node2_index);
        }
        if(!node1_is_src)
        {
            int node1_index = (int)node1->getIndex(a);
            BaseOperation::remConnection(node1->getOperation(), node1_index);
        }
    }
    else
    {
        std::cout << "Error: nodes have incompatible type" << std::endl;
    }
}

//--------------------------------------------------------------
size_t OperationCollection::getId(BaseOperationNode* operation) const
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
size_t OperationCollection::addOperation(std::unique_ptr<BaseOperationNode>& operation)
{
    size_t id = getFreeId();
    operations[id] = std::move(operation);
    return id;
}

//--------------------------------------------------------------
void OperationCollection::setOperation(size_t id, std::unique_ptr<BaseOperationNode>& operation)
{
    operations[id] = std::move(operation);
}

//--------------------------------------------------------------
BaseOperationNode* OperationCollection::getOperation(size_t id)
{
    auto it = operations.find(id);
    if (it != operations.end()) return it->second.get();
    return nullptr;
}

Rect OperationCollection::getBoundingBox() const
{
    Rect ret;
    if(!operations.empty())
    {
        ret.p0 = operations.begin()->second->position;
        ret.p1 = ret.p0 + operations.begin()->second->size;
    }
    for(const auto& op : operations)
    {
        vec2 p = op.second->position;
        vec2 s = op.second->size;
        ret = ret.extends(p);
        ret = ret.extends(p + s);
    }
    return ret;
}

void OperationCollection::centerNodes(const Rect& area)
{
    Rect nodeBox = getBoundingBox();
    vec2 oft = (area.size() - nodeBox.size()) * 0.5f;
    for(auto& op : operations) op.second->position += oft - nodeBox.p0 + area.p0;
}