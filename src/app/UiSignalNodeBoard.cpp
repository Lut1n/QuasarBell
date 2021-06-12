#include "UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

#include "Audio/WavExporter.hpp"

#include "App/SignalNode.hpp"

#include "ReaderWriter/FileIO.hpp"
#include "ReaderWriter/RwHelpers.hpp"
#include "ReaderWriter/SignalOperationsRW.hpp"

#include "SignalOperation/OperationType.hpp"
#include "SignalOperation/Operations.hpp"

#include "ImageOperation/ImageOperationType.hpp"
#include "ImageOperation/ImageOperation.hpp"

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
        nodeboard = std::make_unique<UiNodeBoard>(vec2(0, 0), RenderInterface::getCurrentTargetSize());
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
            operations.centerNodes(Rect::fromPosAndSize(vec2(0.0f,0.0f),nodeboard->size));
            for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
            for(auto co : connections.entries)
            {
                uiConnections->createLink(operations.getOperation(co.src)->outputs[co.src_index].get(), operations.getOperation(co.dst)->inputs[co.dst_index].get());
            }
        }
        _ready = true;
    }

    nodeboard->size = RenderInterface::getCurrentTargetSize();
    
    if(app.resetProject)
    {
        for(auto& op : operations.operations) nodeboard->rem(op.second.get());
        for(auto& op: imageOperations.operations) nodeboard->rem(op.second.get());
        operations.operations.clear();
        imageOperations.operations.clear();
        app.resetProject = false;
    }

    if(app.fileInput.confirmed)
    {
        if(app.fileInput.request == UserFileInput::Load_Prj)
        {
            for(auto& op : operations.operations) nodeboard->rem(op.second.get());
            for(auto& op: imageOperations.operations) nodeboard->rem(op.second.get());
            operations.operations.clear();
            imageOperations.operations.clear();
            OperationConnections connections;
            JsonValue root = loadJsonFile(app.fileInput.filepath);
            loadFrom(root, operations, connections);
            operations.centerNodes(Rect::fromPosAndSize(vec2(0.0f,0.0f),nodeboard->size));
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
        else
        {
            ++it;
        }
    }

    
    auto& icol = imageOperations.operations;
    for(auto it = icol.begin(); it != icol.end();)
    {
        if (!it->second)
        {
            it = icol.erase(it);
        }
        else if (it->second->toDelete)
        {
            nodeboard->rem(it->second.get());
            it = icol.erase(it);
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
        std::unique_ptr<SignalNode> u;
        u.reset( Factory<SignalNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = operations.addOperation(u);
        nodeboard->add(operations.getOperation(id), true, true);
    }
    if(category == NodeCategory_Image && type != qb::ImageOperationType_None && type < qb::ImageOperationType_Count)
    {
        std::unique_ptr<ImageNode> u;
        u.reset( Factory<ImageNode>::create(type) );
        u->position = nodeboard->contextMenuPosition;
        size_t id = imageOperations.addOperation(u);
        nodeboard->add(imageOperations.getOperation(id), true, true);
    }
    app.nodeToCreateCategory = NodeCategory_None;
}

void UiSignalNodeBoard::initializePreviews()
{
    auto& icol = imageOperations.operations;
    for(auto it = icol.begin(); it!=icol.end(); ++it)
    {
        if (!it->second) continue;
        it->second->initializePreview();
    }
}

void UiSignalNodeBoard::updatePreviews()
{
    if(RenderInterface::getTime() - _lastUpdateTime < 0.0) return;
    _lastUpdateTime = RenderInterface::getTime();

    auto& icol = imageOperations.operations;
    for (auto& op : icol) op.second->updatePreview();
}

void UiSignalNodeBoard::onConnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;
    if(dynamic_cast<ImageNode*>(node1) && dynamic_cast<ImageNode*>(node2))
    {
        bool node1_is_src = !a->isInput;

        int node1_index = (int)node1->getIndex(a);
        int node2_index = (int)node2->getIndex(b);

        ImageOperation* op1 = dynamic_cast<ImageNode*>(node1)->getOperation();
        ImageOperation* op2 = dynamic_cast<ImageNode*>(node2)->getOperation();

        if(node1_is_src)
            ImageOperation::setConnection(op1, node1_index, op2, node2_index);
        else
            ImageOperation::setConnection(op2, node2_index, op1, node1_index);
    }
    else if(dynamic_cast<SignalNode*>(node1) && dynamic_cast<SignalNode*>(node2))
    {
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
    else
    {
        std::cout << "Error: nodes have incompatible type" << std::endl;
    }
}

void UiSignalNodeBoard::onDisconnect(UiPin* a, UiPin* b)
{
    auto* node1 = a->parentNode;
    auto* node2 = b->parentNode;

    auto asImage1 = dynamic_cast<ImageNode*>(node1);
    auto asImage2 = dynamic_cast<ImageNode*>(node2);
    auto asSignal1 = dynamic_cast<SignalNode*>(node1);
    auto asSignal2 = dynamic_cast<SignalNode*>(node2);

    if(asImage1 && asImage2)
    {
        bool node1_is_src = !a->isInput;

        if(node1_is_src)
        {
            int node2_index = (int)node2->getIndex(b);
            ImageOperation::remConnection(asImage2->getOperation(), node2_index);
        }
        if(!node1_is_src)
        {
            int node1_index = (int)node1->getIndex(a);
            ImageOperation::remConnection(asImage1->getOperation(), node1_index);
        }
    }
    else if(asSignal1 && asSignal2)
    {
        bool node1_is_src = !a->isInput;

        if(node1_is_src)
        {
            int node2_index = (int)node2->getIndex(b);
            SignalOperation::remConnection(asSignal2->getOperation(), node2_index);
        }
        if(!node1_is_src)
        {
            int node1_index = (int)node1->getIndex(a);
            SignalOperation::remConnection(asSignal1->getOperation(), node1_index);
        }
    }
    else
    {
        std::cout << "Error: nodes have incompatible type" << std::endl;
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
    for(auto& op : operations) op.second->position += oft;
}

//--------------------------------------------------------------
size_t ImageOperationCollection::getId(ImageNode* operation) const
{
    auto it = std::find_if(operations.begin(), operations.end(), [operation](auto& pair) {return operation == pair.second.get();});
    if (it == operations.end()) return 0; // todo: do not that
    return it->first;
}

//--------------------------------------------------------------
size_t ImageOperationCollection::getFreeId() const
{
    size_t id = 0;
    while ( operations.find(id) != operations.end() ) ++id;
    return id;
}

//--------------------------------------------------------------
size_t ImageOperationCollection::addOperation(std::unique_ptr<ImageNode>& operation)
{
    size_t id = getFreeId();
    operations[id] = std::move(operation);
    return id;
}

//--------------------------------------------------------------
void ImageOperationCollection::setOperation(size_t id, std::unique_ptr<ImageNode>& operation)
{
    operations[id] = std::move(operation);
}

//--------------------------------------------------------------
ImageNode* ImageOperationCollection::getOperation(size_t id)
{
    auto it = operations.find(id);
    if (it != operations.end()) return it->second.get();
    return nullptr;
}