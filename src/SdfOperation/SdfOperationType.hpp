#ifndef QUASAR_BELL_RM_OPERATION_TYPE_HPP
#define QUASAR_BELL_RM_OPERATION_TYPE_HPP

#include <vector>
#include <string>

//--------------------------------------------------------------
namespace qb
{
    enum SdfOperationType
    {
        SdfOperationType_Sphere = 0,
        SdfOperationType_Box,
        SdfOperationType_RoundBox,
        SdfOperationType_Torus,
        SdfOperationType_HexPrism,
        SdfOperationType_TriPrism,
        SdfOperationType_Capsule,
        SdfOperationType_Cone,
        SdfOperationType_RoundedCone,
        SdfOperationType_Cylinder,
        SdfOperationType_RoundedCylinder,
        SdfOperationType_Octahedron,
        SdfOperationType_Pyramid,
        SdfOperationType_Union,
        SdfOperationType_Substraction,
        SdfOperationType_Intersection,
        SdfOperationType_SmoothUnion,
        SdfOperationType_SmoothSubstraction,
        SdfOperationType_SmoothIntersection,
        SdfOperationType_Transform,
        SdfOperationType_HighResOutput,

        SdfOperationType_Count,

        SdfOperationType_None
    };

    std::string getSdfOperationName(SdfOperationType type);
    SdfOperationType getSdfOperationType(const std::string& name);
    std::vector<std::string> getSdfOperationNames();
};


#endif // QUASAR_BELL_RM_OPERATION_TYPE_HPP