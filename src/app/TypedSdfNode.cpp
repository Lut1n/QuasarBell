#include "App/TypedSdfNode.hpp"

MAKE_GEOMETRY_NODE_CREATOR(Sphere, qb::SdfOperationType_Sphere)
MAKE_GEOMETRY_NODE_CREATOR(Box, qb::SdfOperationType_Box)
MAKE_GEOMETRY_NODE_CREATOR(RoundBox, qb::SdfOperationType_RoundBox)
MAKE_GEOMETRY_NODE_CREATOR(Torus, qb::SdfOperationType_Torus)
MAKE_GEOMETRY_NODE_CREATOR(HexagonalPrism, qb::SdfOperationType_HexPrism)
MAKE_GEOMETRY_NODE_CREATOR(TriangularPrism, qb::SdfOperationType_TriPrism)
MAKE_GEOMETRY_NODE_CREATOR(Capsule, qb::SdfOperationType_Capsule)
MAKE_GEOMETRY_NODE_CREATOR(Cone, qb::SdfOperationType_Cone)
MAKE_GEOMETRY_NODE_CREATOR(Union, qb::SdfOperationType_Union)
MAKE_GEOMETRY_NODE_CREATOR(Substraction, qb::SdfOperationType_Substraction)
MAKE_GEOMETRY_NODE_CREATOR(Intersection, qb::SdfOperationType_Intersection)
MAKE_GEOMETRY_NODE_CREATOR(SmoothUnion, qb::SdfOperationType_SmoothUnion)
MAKE_GEOMETRY_NODE_CREATOR(SmoothSubstraction, qb::SdfOperationType_SmoothSubstraction)
MAKE_GEOMETRY_NODE_CREATOR(SmoothIntersection, qb::SdfOperationType_SmoothIntersection)
MAKE_GEOMETRY_NODE_CREATOR(Transform, qb::SdfOperationType_Transform)
MAKE_GEOMETRY_NODE_CREATOR(HighResSdfOutput, qb::SdfOperationType_HighResOutput)
