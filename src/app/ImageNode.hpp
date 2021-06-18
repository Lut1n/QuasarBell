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
struct ImageNode : public BaseOperationNode
{
    ImageNode(qb::ImageOperationType opType, BaseOperation* operation)
        : BaseOperationNode(qb::getImageOperationName(opType), (size_t)opType, operation)
        {}
    
    void updatePreview() override;
    void drawPreview(const Rect& previewArea) override;
};
//--------------------------------------------------------------
template<typename OpObject, qb::ImageOperationType OpType>
struct TypedImageNode : public ImageNode
{
    TypedImageNode() : ImageNode(OpType, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE(opclass, optype)\
    struct opclass ## ImageNode : public TypedImageNode<opclass, optype> {};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE_CREATOR(opclass, optype)\
    static TypedCreator<ImageNode, opclass ## ImageNode> opclass ## ImageNodeCreator(optype);


#endif // GUI_IMAGE_OP_NODE_H
