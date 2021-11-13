#include "ReaderWriter/SignalOperationsRW.hpp"

#include "ReaderWriter/RwHelpers.hpp"

#include "SignalOperation/OperationType.hpp"
#include "ImageOperation/ImageOperationType.hpp"

#include "App/SignalNode.hpp"
#include "App/ImageNode.hpp"
#include "App/UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co, const std::string& category, size_t typeFlags, std::function<std::string(BaseOperationNode*)> typeIdGetter)
{
    Rect boxed = collection.getBoundingBox();
    auto& jsonOp = root.setPath(category,"operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto it = ops.begin(); it != ops.end(); ++it)
    {
        if(it->second->getOperation()->getDefaultTypeFlags() != typeFlags) continue;
        auto& jNode = jsonOp.setPath(index++);
        std::string typeName = typeIdGetter(it->second.get());
        jNode.setPath("type").set(typeName);
        jNode.setPath("id").set((float)it->first);
        toJson(jNode.setPath("position"), it->second->position-boxed.p0);
        
        auto op = it->second->getOperation();
        for(size_t i=0; i<op->getPropertyCount(); ++i)
        {
            float f = 0.0f;
            int k = 0;
            bool b = false;
            if (op->getPropertyType(i) == BaseOperationDataType::Float)
            {
                op->getProperty(i, f);
                jNode.setPath("properties", op->getPropertyName(i)).set(f);
            }
            else if (op->getPropertyType(i) == BaseOperationDataType::Int)
            {
                op->getProperty(i, k);
                jNode.setPath("properties", op->getPropertyName(i)).set((float)k);
            }
            else if (op->getPropertyType(i) == BaseOperationDataType::Bool)
            {
                op->getProperty(i, b);
                jNode.setPath("properties", op->getPropertyName(i)).set(b);
            }
        }

        if(op->hasCustomData())
        {
            op->saveCustomData(jNode.setPath("data"));
        }
    }

    auto& jsonCo = root.setPath(category,"connections");
    int coIndex = 0;
    for (auto entry : co.entries)
    {
        jsonCo.setPath(coIndex, "src").set(entry.src);
        jsonCo.setPath(coIndex, "src-pin").set(entry.src_index);
        jsonCo.setPath(coIndex, "dst").set(entry.dst);
        jsonCo.setPath(coIndex, "dst-pin").set(entry.dst_index);
        coIndex++;
    }
}

void loadFrom(JsonValue& root, OperationCollection& collection, OperationConnections& co, const std::string& category, size_t typeFlags, std::function<BaseOperationNode*(const std::string&)> nodeCreator)
{
    auto& jsonOp = root.setPath(category,"operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto& jNode : jsonOp.array.values)
    {
        // if(it->second->getOperation()->getPinType() != pinType) continue;
        int id = (int) jNode.setPath("id").getNumeric();
        std::string type = jNode.setPath("type").getString();
        vec2 position;
        jsonTo(jNode.setPath("position"), position);
        std::unique_ptr<BaseOperationNode> ptr;
        ptr.reset( nodeCreator(type) );
        
        auto op = ptr->getOperation();
        for(size_t i=0; i<op->getPropertyCount(); ++i)
        {
            float f = 0.0f;
            int k = 0;
            bool b = false;
            if (op->getPropertyType(i) == BaseOperationDataType::Float)
            {
                f = (float) jNode.setPath("properties", op->getPropertyName(i)).getNumeric();
                op->setProperty(i, f);
            }
            else if (op->getPropertyType(i) == BaseOperationDataType::Int)
            {
                k = (int) jNode.setPath("properties", op->getPropertyName(i)).getNumeric();
                op->setProperty(i, k);
            }
            else if (op->getPropertyType(i) == BaseOperationDataType::Bool)
            {
                b = jNode.setPath("properties", op->getPropertyName(i)).getBoolean();
                op->setProperty(i, b);
            }
        }

        if(op->hasCustomData())
        {
            op->loadCustomData(jNode.setPath("data"));
        }
        ptr->position = position;
        ops[id] = std::move(ptr);
    }

    auto& jsonCo = root.setPath(category,"connections");
    for(auto& jco : jsonCo.array.values)
    {
        OperationConnections::Entry entry;
        entry.src = (int) jco.setPath("src").getNumeric();
        entry.src_index = (int) jco.setPath("src-pin").getNumeric();
        entry.dst = (int) jco.setPath("dst").getNumeric();
        entry.dst_index = (int) jco.setPath("dst-pin").getNumeric();
        co.entries.push_back(entry);
    }
}