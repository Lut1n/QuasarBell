#ifndef GUI_TYPED_RM_NODE_H
#define GUI_TYPED_RM_NODE_H

#include "App/SdfNode.hpp"
#include "SdfOperation/SdfOperation.hpp"
#include "SdfOperation/SdfPrimitives.hpp"
#include "SdfOperation/SdfTransforms.hpp"
#include "SdfOperation/SdfSampling.hpp"
#include "SdfOperation/SdfBasicOperations.hpp"

MAKE_GEOMETRY_NODE(Sphere, qb::SdfOperationType_Sphere)
MAKE_GEOMETRY_NODE(Box, qb::SdfOperationType_Box)
MAKE_GEOMETRY_NODE(RoundBox, qb::SdfOperationType_RoundBox)
MAKE_GEOMETRY_NODE(Torus, qb::SdfOperationType_Torus)
MAKE_GEOMETRY_NODE(HexagonalPrism, qb::SdfOperationType_HexPrism)
MAKE_GEOMETRY_NODE(TriangularPrism, qb::SdfOperationType_TriPrism)
MAKE_GEOMETRY_NODE(Capsule, qb::SdfOperationType_Capsule)
MAKE_GEOMETRY_NODE(Cone, qb::SdfOperationType_Cone)
MAKE_GEOMETRY_NODE(RoundedCone, qb::SdfOperationType_RoundedCone)
MAKE_GEOMETRY_NODE(Cylinder, qb::SdfOperationType_Cylinder)
MAKE_GEOMETRY_NODE(RoundedCylinder, qb::SdfOperationType_RoundedCylinder)
MAKE_GEOMETRY_NODE(Octahedron, qb::SdfOperationType_Octahedron)
MAKE_GEOMETRY_NODE(Pyramid, qb::SdfOperationType_Pyramid)
MAKE_GEOMETRY_NODE(Union, qb::SdfOperationType_Union)
MAKE_GEOMETRY_NODE(Substraction, qb::SdfOperationType_Substraction)
MAKE_GEOMETRY_NODE(Intersection, qb::SdfOperationType_Intersection)
MAKE_GEOMETRY_NODE(SmoothUnion, qb::SdfOperationType_SmoothUnion)
MAKE_GEOMETRY_NODE(SmoothSubstraction, qb::SdfOperationType_SmoothSubstraction)
MAKE_GEOMETRY_NODE(SmoothIntersection, qb::SdfOperationType_SmoothIntersection)
MAKE_GEOMETRY_NODE(Transform, qb::SdfOperationType_Transform)
MAKE_GEOMETRY_NODE(Repetition, qb::SdfOperationType_Repetition)
MAKE_GEOMETRY_NODE(Twist, qb::SdfOperationType_Twist)
MAKE_GEOMETRY_NODE(Bend, qb::SdfOperationType_Bend)
MAKE_GEOMETRY_NODE(Elongation, qb::SdfOperationType_Elongation)
MAKE_GEOMETRY_NODE(Symmetry, qb::SdfOperationType_Symmetry)
MAKE_GEOMETRY_NODE(Displacement, qb::SdfOperationType_Displacement)
MAKE_GEOMETRY_NODE(Texturing, qb::SdfOperationType_Texturing)
MAKE_GEOMETRY_NODE(HighResSdfOutput, qb::SdfOperationType_HighResOutput)


#endif // GUI_TYPED_RM_NODE_H
