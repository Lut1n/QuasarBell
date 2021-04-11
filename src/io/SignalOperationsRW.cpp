#include "io/SignalOperationsRW.hpp"

#include "io/RwHelpers.hpp"

#include "gui/nodal/AddSignalNode.hpp"
#include "gui/nodal/DebuggerNode.hpp"
#include "gui/nodal/FloatSignalNode.hpp"
#include "gui/nodal/LinearSamplerNode.hpp"
#include "gui/nodal/OscillatorNode.hpp"
#include "gui/nodal/QuantizerNode.hpp"
#include "gui/nodal/MixNode.hpp"
#include "gui/nodal/EnvelopNode.hpp"

#include "NodalEditor.hpp"

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co)
{
    auto& jsonOp = root.setPath("sfx-nodal","operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto it = ops.begin(); it != ops.end(); ++it)
    {
        auto& jNode = jsonOp.setPath(index++);
        if (dynamic_cast<AddSignalNode*>(it->second.get())) jNode.setPath("type").set(std::string("add"));
        if (dynamic_cast<MultSignalNode*>(it->second.get())) jNode.setPath("type").set(std::string("mult"));
        if (dynamic_cast<DebuggerNode*>(it->second.get())) jNode.setPath("type").set(std::string("debugger"));
        if (dynamic_cast<FloatSignalNode*>(it->second.get())) jNode.setPath("type").set(std::string("float"));
        if (dynamic_cast<LinearSamplerNode*>(it->second.get())) jNode.setPath("type").set(std::string("sampler"));
        if (dynamic_cast<OscillatorNode*>(it->second.get())) jNode.setPath("type").set(std::string("oscillator"));
        if (dynamic_cast<QuantizerNode*>(it->second.get())) jNode.setPath("type").set(std::string("quantizer"));
        if (dynamic_cast<MixNode*>(it->second.get())) jNode.setPath("type").set(std::string("mix"));
        if (dynamic_cast<EnvelopNode*>(it->second.get())) jNode.setPath("type").set(std::string("envelop"));
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
        if (type == "add")
            ptr = std::make_unique<AddSignalNode>(position);
        else if (type == "mult")
            ptr = std::make_unique<MultSignalNode>(position);
        else if (type == "debugger")
            ptr = std::make_unique<DebuggerNode>(position);
        else if (type == "float")
            ptr = std::make_unique<FloatSignalNode>(position);
        else if (type == "sampler")
            ptr = std::make_unique<LinearSamplerNode>(position);
        else if (type == "oscillator")
            ptr = std::make_unique<OscillatorNode>(position);
        else if (type == "quantizer")
            ptr = std::make_unique<QuantizerNode>(position);
        else if (type == "mix")
            ptr = std::make_unique<MixNode>(position);
        else if (type == "envelop")
            ptr = std::make_unique<EnvelopNode>(position);
            
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
