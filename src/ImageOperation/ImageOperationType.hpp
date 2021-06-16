#ifndef QUASAR_BELL_IMAGE_OPERATION_TYPE_HPP
#define QUASAR_BELL_IMAGE_OPERATION_TYPE_HPP

#include <vector>
#include <string>

//--------------------------------------------------------------
namespace qb
{
    enum ImageOperationType
    {
        ImageOperationType_HighResOutput = 0,
        ImageOperationType_Color,
        ImageOperationType_Construct3f,
        ImageOperationType_Split3f,
        ImageOperationType_Add,
        ImageOperationType_Sub,
        ImageOperationType_Mult,
        ImageOperationType_Div,
        ImageOperationType_Mix,
        ImageOperationType_Clamp,
        ImageOperationType_Dot,
        ImageOperationType_Cross,
        ImageOperationType_Step,
        ImageOperationType_Pow,
        ImageOperationType_Sqrt,
        ImageOperationType_Abs,
        ImageOperationType_Sin,
        ImageOperationType_Cos,
        ImageOperationType_Tan,
        ImageOperationType_Mod,
        ImageOperationType_Log,
        ImageOperationType_Exp,
        ImageOperationType_Min,
        ImageOperationType_Max,
        ImageOperationType_WhiteNoise,
        ImageOperationType_ValueNoise,
        ImageOperationType_GradientNoise,
        ImageOperationType_SimplexNoise,
        ImageOperationType_Voronoi,
        ImageOperationType_Mandelbrot,
        ImageOperationType_Dynamics,
        ImageOperationType_UvDistortion,
        ImageOperationType_UvInput,
        ImageOperationType_UvMapping,
        ImageOperationType_Blur,
        ImageOperationType_Sharpen,
        ImageOperationType_Morpho,
        ImageOperationType_BumpToNormal,
        ImageOperationType_DirSignal,
        ImageOperationType_RadSignal,
        ImageOperationType_Time,

        ImageOperationType_Count,

        ImageOperationType_None
    };

    std::string getImageOperationName(ImageOperationType type);
    ImageOperationType getImageOperationType(const std::string& name);
    std::vector<std::string> getImageOperationNames();
};


#endif // QUASAR_BELL_IMAGE_OPERATION_TYPE_HPP