#ifndef GUI_TYPED_RM_NODE_H
#define GUI_TYPED_RM_NODE_H

#include "App/SdfNode.hpp"
#include "SdfOperation/SdfOperation.hpp"
#include "SdfOperation/SdfPrimitives.hpp"
#include "SdfOperation/SdfTransforms.hpp"
#include "SdfOperation/SdfBasicOperations.hpp"

MAKE_GEOMETRY_NODE(Sphere, qb::SdfOperationType_Sphere)
MAKE_GEOMETRY_NODE(Box, qb::SdfOperationType_Box)
MAKE_GEOMETRY_NODE(Capsule, qb::SdfOperationType_Capsule)
MAKE_GEOMETRY_NODE(Cone, qb::SdfOperationType_Cone)
MAKE_GEOMETRY_NODE(Union, qb::SdfOperationType_Union)
MAKE_GEOMETRY_NODE(Substraction, qb::SdfOperationType_Substraction)
MAKE_GEOMETRY_NODE(Intersection, qb::SdfOperationType_Intersection)
MAKE_GEOMETRY_NODE(SmoothUnion, qb::SdfOperationType_SmoothUnion)
MAKE_GEOMETRY_NODE(SmoothSubstraction, qb::SdfOperationType_SmoothSubstraction)
MAKE_GEOMETRY_NODE(SmoothIntersection, qb::SdfOperationType_SmoothIntersection)
MAKE_GEOMETRY_NODE(Transform, qb::SdfOperationType_Transform)
MAKE_GEOMETRY_NODE(HighResSdfOutput, qb::SdfOperationType_HighResOutput)


#endif // GUI_TYPED_RM_NODE_H
