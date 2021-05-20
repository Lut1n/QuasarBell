#include "App/TypedImageNode.hpp"

MAKE_IMAGE_NODE_CREATOR(ColorInput, qb::ImageOperationType_Color)
MAKE_IMAGE_NODE_CREATOR(ImageMult, qb::ImageOperationType_Mult)
MAKE_IMAGE_NODE_CREATOR(ImageAdd, qb::ImageOperationType_Add)
MAKE_IMAGE_NODE_CREATOR(WhiteNoise, qb::ImageOperationType_WhiteNoise)
MAKE_IMAGE_NODE_CREATOR(PerlinNoise, qb::ImageOperationType_Perlin)
MAKE_IMAGE_NODE_CREATOR(Dynamics, qb::ImageOperationType_Dynamics)
MAKE_IMAGE_NODE_CREATOR(ImageMix, qb::ImageOperationType_Mix)
MAKE_IMAGE_NODE_CREATOR(HighResOutput, qb::ImageOperationType_HighResOutput)
MAKE_IMAGE_NODE_CREATOR(UvDistortion, qb::ImageOperationType_UvDistortion)
MAKE_IMAGE_NODE_CREATOR(BlurFilter, qb::ImageOperationType_BlurFilter)
MAKE_IMAGE_NODE_CREATOR(BumpToNormal, qb::ImageOperationType_BumpToNormal)
MAKE_IMAGE_NODE_CREATOR(UvMap, qb::ImageOperationType_UvMap)
