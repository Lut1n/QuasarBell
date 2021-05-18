#ifndef TYPED_IMAGE_NODE_H
#define TYPED_IMAGE_NODE_H

#include "App/ImageNode.hpp"
#include "ImageOperation/HighResOutput.hpp"

MAKE_IMAGE_NODE(ColorInput, qb::ImageOperationType_Color)
MAKE_IMAGE_NODE(ImageMult, qb::ImageOperationType_Mult)
MAKE_IMAGE_NODE(ImageAdd, qb::ImageOperationType_Add)
MAKE_IMAGE_NODE(WhiteNoise, qb::ImageOperationType_WhiteNoise)
MAKE_IMAGE_NODE(PerlinNoise, qb::ImageOperationType_Perlin)
MAKE_IMAGE_NODE(Dynamics, qb::ImageOperationType_Dynamics)
MAKE_IMAGE_NODE(ImageMix, qb::ImageOperationType_Mix)
MAKE_IMAGE_NODE(HighResOutput, qb::ImageOperationType_HighResOutput)
MAKE_IMAGE_NODE(UvDistortion, qb::ImageOperationType_UvDistortion)
MAKE_IMAGE_NODE(UvMap, qb::ImageOperationType_UvMap)


#endif // TYPED_IMAGE_NODE_H
