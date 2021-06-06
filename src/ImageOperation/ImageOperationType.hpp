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
        ImageOperationType_WhiteNoise,
        ImageOperationType_Perlin,
        ImageOperationType_Voronoi,
        ImageOperationType_Dynamics,
        ImageOperationType_UvDistortion,
        ImageOperationType_UvInput,
        ImageOperationType_UvMapping,
        ImageOperationType_BlurFilter,
        ImageOperationType_BumpToNormal,
        ImageOperationType_DirSignal,
        ImageOperationType_RadSignal,

        ImageOperationType_Count,

        ImageOperationType_None
    };

    std::string getImageOperationName(ImageOperationType type);
    ImageOperationType getImageOperationType(const std::string& name);
    std::vector<std::string> getImageOperationNames();
};


#endif // QUASAR_BELL_IMAGE_OPERATION_TYPE_HPP