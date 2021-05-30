#ifndef TYPED_IMAGE_NODE_H
#define TYPED_IMAGE_NODE_H

#include "App/ImageNode.hpp"
#include "ImageOperation/ImageBasicOperators.hpp"
#include "ImageOperation/ImageNoises.hpp"
#include "ImageOperation/ImageSampling.hpp"
#include "ImageOperation/ImageUvOperations.hpp"
#include "ImageOperation/HighResOutput.hpp"

MAKE_IMAGE_NODE(ColorInput, qb::ImageOperationType_Color)
MAKE_IMAGE_NODE(ImageAdd, qb::ImageOperationType_Add)
MAKE_IMAGE_NODE(ImageSub, qb::ImageOperationType_Sub)
MAKE_IMAGE_NODE(ImageMult, qb::ImageOperationType_Mult)
MAKE_IMAGE_NODE(ImageDiv, qb::ImageOperationType_Div)
MAKE_IMAGE_NODE(ImageMix, qb::ImageOperationType_Mix)
MAKE_IMAGE_NODE(ImageClamp, qb::ImageOperationType_Clamp)
MAKE_IMAGE_NODE(WhiteNoise, qb::ImageOperationType_WhiteNoise)
MAKE_IMAGE_NODE(PerlinNoise, qb::ImageOperationType_Perlin)
MAKE_IMAGE_NODE(VoronoiNoise, qb::ImageOperationType_Voronoi)
MAKE_IMAGE_NODE(Dynamics, qb::ImageOperationType_Dynamics)
MAKE_IMAGE_NODE(HighResOutput, qb::ImageOperationType_HighResOutput)
MAKE_IMAGE_NODE(UvDistortion, qb::ImageOperationType_UvDistortion)
MAKE_IMAGE_NODE(BlurFilter, qb::ImageOperationType_BlurFilter)
MAKE_IMAGE_NODE(BumpToNormal, qb::ImageOperationType_BumpToNormal)
MAKE_IMAGE_NODE(UvMapping, qb::ImageOperationType_UvMapping)
MAKE_IMAGE_NODE(UvInput, qb::ImageOperationType_UvInput)
MAKE_IMAGE_NODE(DirectionalSignal, qb::ImageOperationType_DirSignal)
MAKE_IMAGE_NODE(RadialSignal, qb::ImageOperationType_RadSignal)


#endif // TYPED_IMAGE_NODE_H
