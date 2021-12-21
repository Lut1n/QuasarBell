#ifndef QB_GEOMETRY_OPERATION_TYPE_HPP
#define QB_GEOMETRY_OPERATION_TYPE_HPP

#include <vector>
#include <string>

//--------------------------------------------------------------
namespace qb
{
    enum GeometryOperationType
    {
        GeometryOperationType_Sphere = 0,
        GeometryOperationType_Box,
        GeometryOperationType_RoundBox,
        GeometryOperationType_Torus,
        GeometryOperationType_HexPrism,
        GeometryOperationType_TriPrism,
        GeometryOperationType_Capsule,
        GeometryOperationType_Cone,
        GeometryOperationType_RoundedCone,
        GeometryOperationType_Cylinder,
        GeometryOperationType_RoundedCylinder,
        GeometryOperationType_Octahedron,
        GeometryOperationType_Pyramid,
        GeometryOperationType_Union,
        GeometryOperationType_Substraction,
        GeometryOperationType_Intersection,
        GeometryOperationType_SmoothUnion,
        GeometryOperationType_SmoothSubstraction,
        GeometryOperationType_SmoothIntersection,
        GeometryOperationType_Transform,
        GeometryOperationType_Repetition,
        GeometryOperationType_Twist,
        GeometryOperationType_Bend,
        GeometryOperationType_Elongation,
        GeometryOperationType_Symmetry,
        GeometryOperationType_Displacement,
        GeometryOperationType_Texturing,
        GeometryOperationType_HighResOutput,
        GeometryOperationType_Voxel,

        GeometryOperationType_Count,

        GeometryOperationType_None
    };

    std::string getGeometryOperationName(GeometryOperationType type);
    GeometryOperationType getGeometryOperationType(const std::string& name);
    std::vector<std::string> getGeometryOperationNames();
};


#endif // QB_GEOMETRY_OPERATION_TYPE_HPP
