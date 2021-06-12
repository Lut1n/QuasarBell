#include "ImageOperation/ImageOperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<ImageOperationType, std::string> s_imageOperationNames = 
    {
        {ImageOperationType_HighResOutput, "highres-output"},
        {ImageOperationType_Color, "color"},
        {ImageOperationType_Add, "add"},
        {ImageOperationType_Sub, "sub"},
        {ImageOperationType_Mult, "mult"},
        {ImageOperationType_Div, "div"},
        {ImageOperationType_Mix, "mix"},
        {ImageOperationType_Clamp, "clamp"},
        {ImageOperationType_Dot, "dot"},
        {ImageOperationType_Cross, "cross"},
        {ImageOperationType_Step, "step"},
        {ImageOperationType_Pow, "pow"},
        {ImageOperationType_Sqrt, "sqrt"},
        {ImageOperationType_Abs, "abs"},
        {ImageOperationType_WhiteNoise, "white-noise"},
        {ImageOperationType_ValueNoise, "value-noise"},
        {ImageOperationType_GradientNoise, "gradient-noise"},
        {ImageOperationType_SimplexNoise, "simplex-noise"},
        {ImageOperationType_Voronoi, "voronoi"},
        {ImageOperationType_Mandelbrot, "mandelbrot"},
        {ImageOperationType_Dynamics, "dynamics"},
        {ImageOperationType_UvDistortion, "uv-distortion"},
        {ImageOperationType_UvMapping, "uv-mapping"},
        {ImageOperationType_UvInput, "uv-input"},
        {ImageOperationType_Blur, "blur"},
        {ImageOperationType_Sharpen, "sharpen"},
        {ImageOperationType_Morpho, "morpho"},
        {ImageOperationType_BumpToNormal, "bump-to-normal"},
        {ImageOperationType_DirSignal, "dir-signal"},
        {ImageOperationType_RadSignal, "rad-signal"},
        {ImageOperationType_Time, "time-input"}
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