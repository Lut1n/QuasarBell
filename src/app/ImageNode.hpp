#ifndef GUI_IMAGE_OP_NODE_H
#define GUI_IMAGE_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"
#include "ImageOperation/ImageOperation.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct ImageNode : public UiNode
{
    ImageNode(const std::string& title, size_t nodetypeId);

    void setOperation(ImageOperation* op);
    ImageOperation* getOperation();

    void displayProperties() override;
    void initializePreview();
    void drawPreview(const Rect& previewArea) override;

    size_t nodeTypeId() const;
private:
    ImageOperation* _operation = nullptr;
    size_t _nodetypeId;
};
//--------------------------------------------------------------
template<typename OpObject, qb::ImageOperationType OpType>
struct TypedImageNode : public ImageNode
{
    TypedImageNode()
        : ImageNode("ImageNode", OpType)
    {
        title->text = qb::getImageOperationName(OpType);
        setOperation(&imageOperation);
    }
private:
    OpObject imageOperation;
};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE(opclass, optype)\
    struct opclass ## ImageNode : public TypedImageNode<opclass, optype> {};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE_CREATOR(opclass, optype)\
    static TypedCreator<ImageNode, opclass ## ImageNode> opclass ## ImageNodeCreator(optype);


#endif // GUI_IMAGE_OP_NODE_H
