#include "App/TypedSdfNode.hpp"

MAKE_GEOMETRY_NODE_CREATOR(Sphere, qb::SdfOperationType_Sphere)
MAKE_GEOMETRY_NODE_CREATOR(Box, qb::SdfOperationType_Box)
MAKE_GEOMETRY_NODE_CREATOR(Union, qb::SdfOperationType_Union)
MAKE_GEOMETRY_NODE_CREATOR(SmoothUnion, qb::SdfOperationType_SmoothUnion)
MAKE_GEOMETRY_NODE_CREATOR(Transform, qb::SdfOperationType_Transform)
MAKE_GEOMETRY_NODE_CREATOR(HighResSdfOutput, qb::SdfOperationType_HighResOutput)
