#include "UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

#include "App/SignalNode.hpp"
#include "App/ImageNode.hpp"
#include "App/GeometryNode.hpp"
#include "SignalOperation/SignalAttributes.hpp"
#include "ImageOperation/ImageAttributes.hpp"
#include "GeometryOperation/GeometryAttributes.hpp"

#include "ReaderWriter/FileIO.hpp"
#include "ReaderWriter/RwHelpers.hpp"
#include "ReaderWriter/SignalOperationsRW.hpp"

#include "SignalOperation/PcmBuilder.hpp"
#include "ImageOperation/TextureBuilder.hpp"

void OperationConnections::fill(UiConnections* ui, const OperationCollection& coll)
{
    for (auto link : ui->links)
    {
        auto co = link.second;
        auto pin1 = co.first;
        auto pin2 = co.second;
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

    auto& col = operations.operations;
    for(auto it = col.begin(); it != col.end();)
    {
        if (it->second->toDelete)
        {
            AppInterface& app = AppInterface::get();
            if(app.getPreviewOperation() == it->second.get())
                app.closeBigPreview();
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
    if(category == NodeCategory_Geometry && type != qb::GeometryOperationType_None && type < qb::GeometryOperationType_Count)
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
    AppInterface::get().closeBigPreview();
}
//--------------------------------------------------------------
void UiSignalNodeBoard::load(const std::string& path)
{
    auto nodeCreator = [](const std::string& type)
    {
        BaseOperationNode* ret = nullptr;
        auto signalNames = qb::getOperationNames();
        auto imageNames = qb::getImageOperationNames();
        auto geometryNames = qb::getGeometryOperationNames();
        if (std::find(signalNames.begin(), signalNames.end(), type) != signalNames.end())
            ret = Factory<SignalNode>::create(qb::getOperationType(type));
        else if (std::find(imageNames.begin(), imageNames.end(), type) != imageNames.end())
            ret = Factory<ImageNode>::create(qb::getImageOperationType(type));
        else if (std::find(geometryNames.begin(), geometryNames.end(), type) != geometryNames.end())
            ret = Factory<GeometryNode>::create(qb::getGeometryOperationType(type));
        return ret;
    };

    cleanup();
    OperationConnections connections;
    JsonValue root = loadJsonFile(path);
    loadFrom(root, operations, connections, nodeCreator);

    operations.centerNodes(Rect::fromPosAndSize(vec2(0.0f,0.0f),nodeboard->size));
    for(auto& op : operations.operations) nodeboard->add(op.second.get(), true, true);
    for(auto co : connections.entries)
    {
        auto op1 = operations.getOperation(co.src);
        auto op2 = operations.getOperation(co.dst);
        if (op1 == nullptr || op2 == nullptr) continue;

        auto pin1 = op1->outputs[co.src_index].get();
        auto pin2 = op2->inputs[co.dst_index].get();
        uiConnections->createLink(pin1, pin2);
    }
    nodeboard->rem(uiConnections);
    nodeboard->add(uiConnections);
}
//--------------------------------------------------------------
void UiSignalNodeBoard::save(const std::string& path)
{
    auto nameGetter = [](BaseOperationNode* node)
    {
        if (dynamic_cast<SignalNode*>(node))
            return qb::getOperationName(static_cast<qb::OperationType>(node->nodeTypeId()));
        else if (dynamic_cast<ImageNode*>(node))
            return qb::getImageOperationName(static_cast<qb::ImageOperationType>(node->nodeTypeId()));
        else if (dynamic_cast<GeometryNode*>(node))
            return qb::getGeometryOperationName(static_cast<qb::GeometryOperationType>(node->nodeTypeId()));
        return std::string("error");
    };

    JsonValue root;
    writeInfo(root);
    OperationConnections connections;
    connections.fill(uiConnections, operations);
    saveInto(root, operations, connections, nameGetter);
    saveJsonFile(path, root);
}


static std::unordered_map<OutputData*, PcmGenerator> generators;
static std::unordered_map<ImageOutput*, TextureGenerator> textureGenerators;
static std::unordered_map<HighResSdfOutputData*, TextureGenerator> sdfGenerators;
static std::unordered_map<VoxelOutputData*, TextureGenerator> voxGenerators;

//--------------------------------------------------------------
void UiSignalNodeBoard::updatePreviews()
{
    if(RenderInterface::getTime() - _lastUpdateTime < 0.0) return;
    _lastUpdateTime = RenderInterface::getTime();

    for(auto& node : operations.operations)
    {
        auto attributes = node.second->getAttributes();
        if (auto output = dynamic_cast<OutputData*>(attributes))
        {
            auto& generator = generators[output];
            generator.compute(node.second.get());
        }
        if (auto texture = dynamic_cast<ImageOutput*>(attributes))
        {
            auto& generator = textureGenerators[texture];
            generator.compute(node.second.get());
        }
        if (auto sdf = dynamic_cast<HighResSdfOutputData*>(attributes))
        {
            auto& generator = sdfGenerators[sdf];
            generator.compute(node.second.get());
        }
        if (auto vox = dynamic_cast<VoxelOutputData*>(attributes))
        {
            auto& generator = voxGenerators[vox];
            generator.compute(node.second.get());
        }

        if (auto texPreview = dynamic_cast<TexturePreview*>(node.second->getPreview()))
        {
            if (texPreview->previewClicked)
                AppInterface::get().openBigPreview(node.second.get());
            texPreview->previewClicked = false;
        }
    }
}
//--------------------------------------------------------------
void UiSignalNodeBoard::onConnect(UiPin* a, UiPin* b)
{
    auto node1 = dynamic_cast<BaseOperationNode*>(a->parentNode);
    auto node2 = dynamic_cast<BaseOperationNode*>(b->parentNode);

    CustomNodeVisitor connectionVisitor([](BaseOperationNode& node){
        node.setPreview(nullptr);
        node.getAttributes()->onChanged();
        node.getAttributes()->onConnectionChanged();
    });

    CleanPreviews cleaner;

    if (node1)
        node1->accept(connectionVisitor);
    if (node2)
        node2->accept(connectionVisitor);
}
//--------------------------------------------------------------
void UiSignalNodeBoard::onDisconnect(UiPin* a, UiPin* b)
{
    auto node1 = dynamic_cast<BaseOperationNode*>(a->parentNode);
    auto node2 = dynamic_cast<BaseOperationNode*>(b->parentNode);

    CustomNodeVisitor connectionVisitor([](BaseOperationNode& node){
        node.setPreview(nullptr);
        node.getAttributes()->onConnectionChanged();
    });

    CleanPreviews cleaner;

    if (node1)
        node1->accept(connectionVisitor);
    if (node2)
        node2->accept(connectionVisitor);
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
