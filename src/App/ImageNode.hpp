#ifndef GUI_IMAGE_OP_NODE_H
#define GUI_IMAGE_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "App/BaseOperationNode.hpp"
#include "ImageOperation/ImageOperationType.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct ImageNode : public BaseOperationNode
{
    ImageNode(size_t opType, BaseAttributes* operation)
        : BaseOperationNode(qb::getImageOperationName((qb::ImageOperationType)opType), opType, operation)
        {
        }
};
//--------------------------------------------------------------
template<typename OpObject>
struct TypedImageNode : public ImageNode
{
    TypedImageNode() : ImageNode(OpObject::TypeId, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE(opclass)\
    struct opclass ## ImageNode : public TypedImageNode<opclass> {};
//--------------------------------------------------------------
#define MAKE_IMAGE_NODE_CREATOR(opclass)\
    static TypedCreator<ImageNode, opclass ## ImageNode> opclass ## ImageNodeCreator(opclass::TypeId);


//--------------------------------------------------------------
/*template<typename Prop>
struct TextureOperation : public BaseOperation
{
    using TargetDataType = Prop;
};*/

#endif // GUI_IMAGE_OP_NODE_H
