#include "ImageOperation/ImageOperationType.hpp"

#include <unordered_map>

//--------------------------------------------------------------
namespace qb
{
    const static std::unordered_map<ImageOperationType, std::string> s_imageOperationNames = 
    {
        {ImageOperationType_HighResOutput, "high-resolution"},
        {ImageOperationType_Color, "color"},
        {ImageOperationType_Construct3f, "struct-3"},
        {ImageOperationType_Split3f, "split-3"},
        {ImageOperationType_Add, "image-add"},
        {ImageOperationType_Sub, "image-sub"},
        {ImageOperationType_Mult, "image-mult"},
        {ImageOperationType_Div, "image-div"},
        {ImageOperationType_Mix, "image-mix"},
        {ImageOperationType_Clamp, "image-clamp"},
        {ImageOperationType_Dot, "image-dot"},
        {ImageOperationType_Cross, "image-cross"},
        {ImageOperationType_Step, "image-step"},
        {ImageOperationType_Pow, "image-pow"},
        {ImageOperationType_Sqrt, "image-sqrt"},
        {ImageOperationType_Abs, "image-abs"},
        {ImageOperationType_Sin, "image-sin"},
        {ImageOperationType_Cos, "image-cos"},
        {ImageOperationType_Tan, "image-tan"},
        {ImageOperationType_Mod, "image-mod"},
        {ImageOperationType_Exp, "image-exp"},
        {ImageOperationType_Log, "image-log"},
        {ImageOperationType_Min, "image-min"},
        {ImageOperationType_Max, "image-max"},
        {ImageOperationType_WhiteNoise, "white-noise"},
        {ImageOperationType_ValueNoise, "value-noise"},
        {ImageOperationType_GradientNoise, "gradient-noise"},
        {ImageOperationType_SimplexNoise, "simplex-noise"},
        {ImageOperationType_Voronoi, "worley-noise"},
        {ImageOperationType_Mandelbrot, "mandelbrot"},
        {ImageOperationType_Dynamics, "dynamics"},
        {ImageOperationType_UvDistortion, "distort-uv"},
        {ImageOperationType_UvMapping, "uv-mapping"},
        {ImageOperationType_UvInput, "uv-input"},
        {ImageOperationType_SphericalCoord, "spherical-coord"},
        {ImageOperationType_Blur, "blur"},
        {ImageOperationType_Sharpen, "sharpen"},
        {ImageOperationType_Morpho, "morpho"},
        {ImageOperationType_BumpToNormal, "to-normal"},
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
