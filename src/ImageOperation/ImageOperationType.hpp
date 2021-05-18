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
        ImageOperationType_Mult,
        ImageOperationType_Add,
        ImageOperationType_WhiteNoise,
        ImageOperationType_Perlin,
        ImageOperationType_Dynamics,
        ImageOperationType_Mix,
        ImageOperationType_UvDistortion,
        ImageOperationType_UvMap,
        // ImageOperationType_BumpToNormal,

        ImageOperationType_Count,

        ImageOperationType_None
    };

    std::string getImageOperationName(ImageOperationType type);
    ImageOperationType getImageOperationType(const std::string& name);
    std::vector<std::string> getImageOperationNames();
};


#endif // QUASAR_BELL_IMAGE_OPERATION_TYPE_HPP