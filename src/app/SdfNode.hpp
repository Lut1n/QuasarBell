#ifndef GUI_RM_OP_NODE_H
#define GUI_RM_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"
#include "SdfOperation/SdfOperation.hpp"
#include "SdfOperation/SdfOperationType.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct GeometryNode : public BaseOperationNode
{
    GeometryNode(qb::SdfOperationType opType, BaseOperation* operation)
        : BaseOperationNode(qb::getSdfOperationName(opType), (size_t)opType, operation)
        {}
    
    void updatePreview() override;
    void drawPreview(const Rect& previewArea) override;
};
//--------------------------------------------------------------
template<typename OpObject, qb::SdfOperationType OpType>
struct TypedGeometryNode : public GeometryNode
{
    TypedGeometryNode() : GeometryNode(OpType, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_GEOMETRY_NODE(opclass, optype)\
    struct opclass ## Node : public TypedGeometryNode<opclass, optype> {};
//--------------------------------------------------------------
#define MAKE_GEOMETRY_NODE_CREATOR(opclass, optype)\
    static TypedCreator<GeometryNode, opclass ## Node> opclass ## Creator(optype);


#endif // GUI_RM_OP_NODE_H
