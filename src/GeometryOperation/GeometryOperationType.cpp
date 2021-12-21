#include "GeometryOperation/GeometryOperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<GeometryOperationType, std::string> s_GeometryOperationNames =
    {
        {GeometryOperationType_Sphere, "sphere"},
        {GeometryOperationType_Box, "box"},
        {GeometryOperationType_RoundBox, "roundbox"},
        {GeometryOperationType_Torus, "torus"},
        {GeometryOperationType_HexPrism, "hexprism"},
        {GeometryOperationType_TriPrism, "triprism"},
        {GeometryOperationType_Capsule, "capsule"},
        {GeometryOperationType_Cone, "cone"},
        {GeometryOperationType_RoundedCone, "roundedcone"},
        {GeometryOperationType_Cylinder, "cylinder"},
        {GeometryOperationType_RoundedCylinder, "roundedcylinder"},
        {GeometryOperationType_Octahedron, "octahedron"},
        {GeometryOperationType_Pyramid, "pyramid"},
        {GeometryOperationType_Union, "union"},
        {GeometryOperationType_Substraction, "geo-sub"},
        {GeometryOperationType_Intersection, "inter"},
        {GeometryOperationType_SmoothUnion, "s-union"},
        {GeometryOperationType_SmoothSubstraction, "s-sub"},
        {GeometryOperationType_SmoothIntersection, "s-inter"},
        {GeometryOperationType_Transform, "transform"},
        {GeometryOperationType_Repetition, "repetition"},
        {GeometryOperationType_Twist, "twist"},
        {GeometryOperationType_Bend, "bend"},
        {GeometryOperationType_Elongation, "elongation"},
        {GeometryOperationType_Symmetry, "symmetry"},
        {GeometryOperationType_Displacement, "displacement"},
        {GeometryOperationType_Texturing, "texturing"},
        {GeometryOperationType_HighResOutput, "high-res"},
        {GeometryOperationType_Voxel, "voxel"}
    };
}
//--------------------------------------------------------------
std::string qb::getGeometryOperationName(GeometryOperationType type)
{
    auto it = s_GeometryOperationNames.find(type);
    if (it != s_GeometryOperationNames.end())
        return it->second;
    return "None";
}
//--------------------------------------------------------------
qb::GeometryOperationType qb::getGeometryOperationType(const std::string& name)
{
    for(auto& pair : s_GeometryOperationNames)
    {
        if (pair.second == name) return pair.first;
    }
    return GeometryOperationType_None;
}
//--------------------------------------------------------------
std::vector<std::string> qb::getGeometryOperationNames()
{
    std::vector<std::string> ret(GeometryOperationType_Count);
    for(size_t i=0; i<(size_t)GeometryOperationType_Count; ++i) ret[i] = getGeometryOperationName((GeometryOperationType)i);
    return ret;
}
