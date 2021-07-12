#include "SdfOperation/SdfOperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<SdfOperationType, std::string> s_SdfOperationNames = 
    {
        {SdfOperationType_Sphere, "sphere"},
        {SdfOperationType_Box, "box"},
        {SdfOperationType_Capsule, "capsule"},
        {SdfOperationType_Cone, "cone"},
        {SdfOperationType_Union, "union"},
        {SdfOperationType_Substraction, "sub"},
        {SdfOperationType_Intersection, "inter"},
        {SdfOperationType_SmoothUnion, "s-union"},
        {SdfOperationType_SmoothSubstraction, "s-sub"},
        {SdfOperationType_SmoothIntersection, "s-inter"},
        {SdfOperationType_Transform, "transform"},
        {SdfOperationType_HighResOutput, "high-res"}
    };
}
//--------------------------------------------------------------
std::string qb::getSdfOperationName(SdfOperationType type)
{
    auto it = s_SdfOperationNames.find(type);
    if (it != s_SdfOperationNames.end())
        return it->second;
    return "None";
}
//--------------------------------------------------------------
qb::SdfOperationType qb::getSdfOperationType(const std::string& name)
{
    for(auto& pair : s_SdfOperationNames)
    {
        if (pair.second == name) return pair.first;
    }
    return SdfOperationType_None;
}
//--------------------------------------------------------------
std::vector<std::string> qb::getSdfOperationNames()
{
    std::vector<std::string> ret(SdfOperationType_Count);
    for(size_t i=0; i<(size_t)SdfOperationType_Count; ++i) ret[i] = getSdfOperationName((SdfOperationType)i);
    return ret;
}