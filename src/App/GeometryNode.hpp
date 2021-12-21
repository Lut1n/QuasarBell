#ifndef GUI_RM_OP_NODE_H
#define GUI_RM_OP_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"
#include "GeometryOperation/GeometryOperationType.hpp"
#include "App/BaseOperationNode.hpp"
#include "Core/Factory.h"

//--------------------------------------------------------------
struct GeometryNode : public BaseOperationNode
{
    GeometryNode(qb::GeometryOperationType opType, BaseAttributes* operation)
        : BaseOperationNode(qb::getGeometryOperationName(opType), (size_t)opType, operation)
        {
        }
};
//--------------------------------------------------------------
template<typename OpObject>
struct TypedGeometryNode : public GeometryNode
{
    TypedGeometryNode() : GeometryNode((qb::GeometryOperationType)OpObject::TypeId, new OpObject()) {}
};
//--------------------------------------------------------------
#define MAKE_GEOMETRY_NODE(opclass)\
    struct opclass ## GeometryNode : public TypedGeometryNode<opclass> {};
//--------------------------------------------------------------
#define MAKE_GEOMETRY_NODE_CREATOR(opclass)\
    static TypedCreator<GeometryNode, opclass ## GeometryNode> opclass ## GeometryNodeCreator(opclass::TypeId);


#endif // GUI_RM_OP_NODE_H
