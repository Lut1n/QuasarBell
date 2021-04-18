#include "io/SignalOperationsRW.hpp"

#include "io/RwHelpers.hpp"

#include "gui/nodal/AddSignalNode.hpp"
#include "gui/nodal/DebuggerNode.hpp"
#include "gui/nodal/FloatSignalNode.hpp"
#include "gui/nodal/CubicSamplerNode.hpp"
#include "gui/nodal/OscillatorNode.hpp"
#include "gui/nodal/QuantizerNode.hpp"
#include "gui/nodal/MixNode.hpp"
#include "gui/nodal/EnvelopNode.hpp"

#include "signal/operations/OperationType.hpp"
#include "NodalEditor.hpp"

#include "Core/Factory.h"

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co)
{
    auto& jsonOp = root.setPath("sfx-nodal","operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto it = ops.begin(); it != ops.end(); ++it)
    {
        auto& jNode = jsonOp.setPath(index++);
        std::string typeName = qb::getOperationName(static_cast<qb::OperationType>(it->second->nodeTypeId()));
        jNode.setPath("type").set(typeName);
        jNode.setPath("id").set((float)it->first);
        toJson(jNode.setPath("position"), it->second->position);
        
        auto op = it->second->getOperation();
        for(size_t i=0; i<op->getPropertyCount(); ++i)
        {
            float f = 0.0f;
            int k = 0;
            bool b = false;
            if (op->getPropertyType(i) == DataType_Float)
            {
                op->getProperty(i, f);
                jNode.setPath("properties", op->getPropertyName(i)).set(f);
            }
            else if (op->getPropertyType(i) == DataType_Int)
            {
                op->getProperty(i, k);
                jNode.setPath("properties", op->getPropertyName(i)).set((float)k);
            }
            else if (op->getPropertyType(i) == DataType_Bool)
            {
                op->getProperty(i, b);
                jNode.setPath("properties", op->getPropertyName(i)).set(b);
            }
        }
    }

    auto& jsonCo = root.setPath("sfx-nodal","connections");
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
//--------------------------------------------------------------
void loadFrom(JsonValue& root, OperationCollection& collection, OperationConnections& co)
{
    auto& jsonOp = root.setPath("sfx-nodal","operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto& jNode : jsonOp.array.values)
    {
        int id = jNode.setPath("id").getNumeric();
        std::string type = jNode.setPath("type").getString();
        vec2 position;
        jsonTo(jNode.setPath("position"), position);
        std::unique_ptr<SignalOperationNode> ptr;
        ptr.reset( Factory<SignalOperationNode>::create(type) );
        
        auto op = ptr->getOperation();
        for(size_t i=0; i<op->getPropertyCount(); ++i)
        {
            float f = 0.0f;
            int k = 0;
            bool b = false;
            if (op->getPropertyType(i) == DataType_Float)
            {
                f = jNode.setPath("properties", op->getPropertyName(i)).getNumeric();
                op->setProperty(i, f);
            }
            else if (op->getPropertyType(i) == DataType_Int)
            {
                k = jNode.setPath("properties", op->getPropertyName(i)).getNumeric();
                op->setProperty(i, k);
            }
            else if (op->getPropertyType(i) == DataType_Bool)
            {
                b = jNode.setPath("properties", op->getPropertyName(i)).getBoolean();
                op->setProperty(i, b);
            }
        }
        ptr->position = position;
        ops[id] = std::move(ptr);
    }

    auto& jsonCo = root.setPath("sfx-nodal","connections");
    for(auto& jco : jsonCo.array.values)
    {
        OperationConnections::Entry entry;
        entry.src = jco.setPath("src").getNumeric();
        entry.src_index = jco.setPath("src-pin").getNumeric();
        entry.dst = jco.setPath("dst").getNumeric();
        entry.dst_index = jco.setPath("dst-pin").getNumeric();
        co.entries.push_back(entry);
    }
}
