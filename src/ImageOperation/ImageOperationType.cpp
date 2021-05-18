#include "ImageOperation/ImageOperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<ImageOperationType, std::string> s_imageOperationNames = 
    {
        {ImageOperationType_HighResOutput, "highres-output"},
        {ImageOperationType_Color, "color"},
        {ImageOperationType_Mult, "mult"},
        {ImageOperationType_Add, "add"},
        {ImageOperationType_WhiteNoise, "white-noise"},
        {ImageOperationType_Perlin, "perlin"},
        {ImageOperationType_Dynamics, "dynamics"},
        {ImageOperationType_UvDistortion, "uv-distortion"},
        {ImageOperationType_UvMap, "uv-map"},
        // {ImageOperationType_BumpToNormal, "bump-to-normal"},
        {ImageOperationType_Mix, "mix"}
    };
}
//--------------------------------------------------------------
std::string qb::getImageOperationName(ImageOperationType type)
{
    auto it = s_imageOperationNames.find(type);
    if (it != s_imageOperationNames.end())
        return it->second;
    return "None";
}
//--------------------------------------------------------------
qb::ImageOperationType qb::getImageOperationType(const std::string& name)
{
    for(auto& pair : s_imageOperationNames)
    {
        if (pair.second == name) return pair.first;
    }
    return ImageOperationType_None;
}
//--------------------------------------------------------------
std::vector<std::string> qb::getImageOperationNames()
{
    std::vector<std::string> ret(ImageOperationType_Count);
    for(size_t i=0; i<(size_t)ImageOperationType_Count; ++i) ret[i] = getImageOperationName((ImageOperationType)i);
    return ret;
}