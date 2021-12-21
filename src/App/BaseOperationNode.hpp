#ifndef GUI_BASE_OPERATION_NODE_H
#define GUI_BASE_OPERATION_NODE_H

#include "App/BaseAttributes.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"

#include "imgui.h"

//--------------------------------------------------------------
struct BasePreview
{
    virtual ~BasePreview() = default;

    virtual void attributePreview()
    {
    }

    virtual void nodePreview(const Rect& previewArea)
    {
    }
};

//--------------------------------------------------------------
struct BaseOperationNode;
struct BaseNodeVisitor
{
    virtual ~BaseNodeVisitor() = default;

    virtual void visit(BaseOperationNode& node) = 0;
};

//--------------------------------------------------------------
struct NodeIoDefinition
{
    std::string name;
    size_t pinTypeFlags = (size_t)BaseAttributes::IoType::Undefined;
};

//--------------------------------------------------------------
struct BaseOperationNode : public UiNode
{
    BaseOperationNode(const std::string& title, size_t nodetypeId, BaseAttributes* properties);

    // void uiDebugIo();

    BaseAttributes* getAttributes() {return _attributes.get();}

    virtual void ui()
    {
        ImGui::Text("Preview:");
        if (_preview)
            _preview->attributePreview();
        else
            ImGui::Text("No preview");
        ImGui::Separator();
        _attributes->ui();
    };

    size_t nodeTypeId() const;

    void accept(BaseNodeVisitor& visitor)
    {
        visitor.visit(*this);
    }

    void setPreview(BasePreview* preview)
    {
        _preview = preview;
    }

    BasePreview* getPreview()
    {
        return _preview;
    }

    void displayAttributes() override { ui(); }
    void drawPreview(const Rect& previewArea) override;

private:
    std::unique_ptr<BaseAttributes> _attributes;
    BasePreview* _preview = nullptr;
    size_t _nodetypeId;
};

//--------------------------------------------------------------
struct CleanPreviews : public BaseNodeVisitor
{
    void visit(BaseOperationNode& node) override
    {
        node.setPreview(nullptr);
        for(size_t i=0; i<node.getSourceCount(); ++i)
        {
            auto source = dynamic_cast<BaseOperationNode*>(node.getSourceNode(i));
            if (source) source->accept(*this);
        }
    }
};

template<typename Functor>
struct CustomNodeVisitor : public BaseNodeVisitor
{
    Functor functor;

    CustomNodeVisitor(Functor f) : functor(f)
    {
    }

    void visit(BaseOperationNode& node) override
    {
        functor(node);
        for(size_t i=0; i<node.getSourceCount(); ++i)
        {
            auto source = dynamic_cast<BaseOperationNode*>(node.getSourceNode(i));
            if (source) source->accept(*this);
        }
    }
};

#endif // GUI_BASE_OPERATION_NODE_H
