#include "App/TypedImageNode.hpp"

MAKE_IMAGE_NODE_CREATOR(ColorInput, qb::ImageOperationType_Color)
MAKE_IMAGE_NODE_CREATOR(ImageAdd, qb::ImageOperationType_Add)
MAKE_IMAGE_NODE_CREATOR(ImageSub, qb::ImageOperationType_Sub)
MAKE_IMAGE_NODE_CREATOR(ImageMult, qb::ImageOperationType_Mult)
MAKE_IMAGE_NODE_CREATOR(ImageDiv, qb::ImageOperationType_Div)
MAKE_IMAGE_NODE_CREATOR(ImageMix, qb::ImageOperationType_Mix)
MAKE_IMAGE_NODE_CREATOR(ImageClamp, qb::ImageOperationType_Clamp)
MAKE_IMAGE_NODE_CREATOR(ImageDot, qb::ImageOperationType_Dot)
MAKE_IMAGE_NODE_CREATOR(ImageCross, qb::ImageOperationType_Cross)
MAKE_IMAGE_NODE_CREATOR(ImageStep, qb::ImageOperationType_Step)
MAKE_IMAGE_NODE_CREATOR(ImagePow, qb::ImageOperationType_Pow)
MAKE_IMAGE_NODE_CREATOR(ImageSqrt, qb::ImageOperationType_Sqrt)
MAKE_IMAGE_NODE_CREATOR(ImageAbs, qb::ImageOperationType_Abs)
MAKE_IMAGE_NODE_CREATOR(WhiteNoise, qb::ImageOperationType_WhiteNoise)
MAKE_IMAGE_NODE_CREATOR(PerlinNoise, qb::ImageOperationType_Perlin)
MAKE_IMAGE_NODE_CREATOR(VoronoiNoise, qb::ImageOperationType_Voronoi)
MAKE_IMAGE_NODE_CREATOR(Dynamics, qb::ImageOperationType_Dynamics)
MAKE_IMAGE_NODE_CREATOR(HighResOutput, qb::ImageOperationType_HighResOutput)
MAKE_IMAGE_NODE_CREATOR(UvDistortion, qb::ImageOperationType_UvDistortion)
MAKE_IMAGE_NODE_CREATOR(BlurFilter, qb::ImageOperationType_BlurFilter)
MAKE_IMAGE_NODE_CREATOR(BumpToNormal, qb::ImageOperationType_BumpToNormal)
MAKE_IMAGE_NODE_CREATOR(UvMapping, qb::ImageOperationType_UvMapping)
MAKE_IMAGE_NODE_CREATOR(UvInput, qb::ImageOperationType_UvInput)
MAKE_IMAGE_NODE_CREATOR(DirectionalSignal, qb::ImageOperationType_DirSignal)
MAKE_IMAGE_NODE_CREATOR(RadialSignal, qb::ImageOperationType_RadSignal)
