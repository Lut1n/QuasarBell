#include "ReaderWriter/SignalOperationsRW.hpp"

#include "ReaderWriter/RwHelpers.hpp"

#include "SignalOperation/OperationType.hpp"
// #include "ImageOperation/ImageOperationType.hpp"

#include "App/SignalNode.hpp"
// #include "App/ImageNode.hpp"
#include "App/UiSignalNodeBoard.hpp"

#include "Core/Factory.h"

//--------------------------------------------------------------
void saveInto(JsonValue& root, OperationCollection& collection, const OperationConnections& co, std::function<std::string(BaseOperationNode*)> typeIdGetter)
{
    Rect boxed = collection.getBoundingBox();
    auto& jsonOp = root.setPath("operations");
    auto& ops = collection.operations;
    int index = 0;
    for(auto it = ops.begin(); it != ops.end(); ++it)
    {
        auto& jNode = jsonOp.setPath(index++);
        std::string typeName = typeIdGetter(it->second.get());
        jNode.setPath("type").set(typeName);
        jNode.setPath("id").set((float)it->first);
        toJson(jNode.setPath("position"), it->second->position-boxed.p0);
        
        auto attributes = it->second->getAttributes();
        for(size_t i=0; i<attributes->count(); ++i)
        {
            auto type = attributes->typeAt(i);
            auto& jattr = jNode.setPath("attributes", attributes->nameAt(i));
            if (type == BaseAttributes::Type::Float)
            {
                float f = 0.0f;
                attributes->get(i, f);
                jattr.set(f);
            }
            else if (type == BaseAttributes::Type::Int)
            {
                int k = 0;
                attributes->get(i, k);
                jattr.set((float)k);
            }
            else if (type == BaseAttributes::Type::Bool)
            {
                bool b = false;
                attributes->get(i, b);
                jattr.set(b);
            }
            else if (type == BaseAttributes::Type::Color3)
            {
                float color[3];
                attributes->get(i, color);
                toJson(jattr, color, 3);
            }
        }

        if(attributes->hasCustomData())
        {
            attributes->saveCustomData(jNode.setPath("data"));
        }
    }

    auto& jsonCo = root.setPath("connections");
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

void loadAttribute(BaseAttributes* attributes, JsonValue& json, const std::string& dataName)
{
    for(size_t i=0; i<attributes->count(); ++i)
    {
        auto type = attributes->typeAt(i);
        auto& jattr = json.setPath(dataName, attributes->nameAt(i));
        if (type == BaseAttributes::Type::Float)
        {
            float f = (float) jattr.getNumeric();
            attributes->set(i, f);
        }
        else if (type == BaseAttributes::Type::Int)
        {
            int k = (int) jattr.getNumeric();
            attributes->set(i, k);
        }
        else if (type == BaseAttributes::Type::Bool)
        {
            bool b = jattr.getBoolean();
            attributes->set(i, b);
        }
        else if (type == BaseAttributes::Type::Color3)
        {
            float color[3];
            if (dataName == "properties")
            {
                color[0] = (float) json.setPath(dataName, "red").getNumeric();
                color[1] = (float) json.setPath(dataName, "green").getNumeric();
                color[2] = (float) json.setPath(dataName, "blue").getNumeric();
            }
            else
            {
                jsonTo(jattr, color, 3);
            }
            attributes->set(i, color);
        }
    }

    if(attributes->hasCustomData())
        attributes->loadCustomData(json.setPath("data"));
}

void loadNodes(OperationCollection& collection, JsonValue& json, const std::string& dataName, std::function<BaseOperationNode*(const std::string&)> nodeCreator)
{
    auto& ops = collection.operations;
    for(auto& jNode : json.array.values)
    {
        std::string type = jNode.setPath("type").getString();
        std::unique_ptr<BaseOperationNode> ptr;
        ptr.reset( nodeCreator(type) );
        
        auto attributes = ptr->getAttributes();
        loadAttribute(attributes, jNode, dataName);
        
        jsonTo(jNode.setPath("position"), ptr->position);
        int id = (int) jNode.setPath("id").getNumeric();
        ops[id] = std::move(ptr);
    }
}

void loadConnections(OperationConnections& co, JsonValue& json)
{
    for(auto& jco : json.array.values)
    {
        OperationConnections::Entry entry;
        entry.src = (int) jco.setPath("src").getNumeric();
        entry.src_index = (int) jco.setPath("src-pin").getNumeric();
        entry.dst = (int) jco.setPath("dst").getNumeric();
        entry.dst_index = (int) jco.setPath("dst-pin").getNumeric();
        co.entries.push_back(entry);
    }
}

void loadRetro4(JsonValue& root, OperationCollection& collection, OperationConnections& co, std::function<BaseOperationNode*(const std::string&)> nodeCreator)
{
    std::array category = {"sfx-nodal", "texture-nodal", "sdf-nodal"};

    for(auto cat : category)
    {
        auto& jsonOp = root.setPath(cat,"operations");

    auto nameConvertor = [cat, &nodeCreator](const std::string& name)
    {
        if (cat == "texture-nodal")
        {
            if (name == "add") return nodeCreator("image-add");
            if (name == "sub") return nodeCreator("image-sub");
            if (name == "mult") return nodeCreator("image-mult");
            if (name == "div") return nodeCreator("image-div");
            if (name == "mix") return nodeCreator("image-mix");
            if (name == "clamp") return nodeCreator("image-clamp");
            if (name == "dot") return nodeCreator("image-dot");
            if (name == "cross") return nodeCreator("image-cross");
            if (name == "step") return nodeCreator("image-step");
            if (name == "pow") return nodeCreator("image-pow");
            if (name == "sqrt") return nodeCreator("image-sqrt");
            if (name == "abs") return nodeCreator("image-abs");
            if (name == "sin") return nodeCreator("image-sin");
            if (name == "cos") return nodeCreator("image-cos");
            if (name == "tan") return nodeCreator("image-tan");
            if (name == "mod") return nodeCreator("image-mod");
            if (name == "exp") return nodeCreator("image-exp");
            if (name == "log") return nodeCreator("image-log");
            if (name == "min") return nodeCreator("image-min");
            if (name == "max") return nodeCreator("image-max");
        }
        if (cat == "sdf-nodal")
        {
            if (name == "sub") return nodeCreator("geo-sub");
        }
        return nodeCreator(name);
    };

        loadNodes(collection, jsonOp, "properties", nameConvertor);
    }

    for(auto cat : category)
    {
        auto& jsonCo = root.setPath(cat,"connections");
        loadConnections(co, jsonCo);
    }
}

void loadFrom(JsonValue& root, OperationCollection& collection, OperationConnections& co, std::function<BaseOperationNode*(const std::string&)> nodeCreator)
{
    auto version = root.setPath("info", "version").getNumeric();
    if (version < 4.1f)
    {
        loadRetro4(root, collection, co, nodeCreator);
        return;
    }

    auto& jsonOp = root.setPath("operations");
    loadNodes(collection, jsonOp, "attributes", nodeCreator);

    auto& jsonCo = root.setPath("connections");
    loadConnections(co, jsonCo);
}
