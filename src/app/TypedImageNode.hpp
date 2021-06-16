#ifndef TYPED_IMAGE_NODE_H
#define TYPED_IMAGE_NODE_H

#include "App/ImageNode.hpp"
#include "ImageOperation/ImageBasicOperators.hpp"
#include "ImageOperation/ImageNoises.hpp"
#include "ImageOperation/ImageSampling.hpp"
#include "ImageOperation/ImageUvOperations.hpp"
#include "ImageOperation/HighResOutput.hpp"

MAKE_IMAGE_NODE(ColorInput, qb::ImageOperationType_Color)
MAKE_IMAGE_NODE(Construct3f, qb::ImageOperationType_Construct3f)
MAKE_IMAGE_NODE(Split3f, qb::ImageOperationType_Split3f)
MAKE_IMAGE_NODE(ImageAdd, qb::ImageOperationType_Add)
MAKE_IMAGE_NODE(ImageSub, qb::ImageOperationType_Sub)
MAKE_IMAGE_NODE(ImageMult, qb::ImageOperationType_Mult)
MAKE_IMAGE_NODE(ImageDiv, qb::ImageOperationType_Div)
MAKE_IMAGE_NODE(ImageMix, qb::ImageOperationType_Mix)
MAKE_IMAGE_NODE(ImageClamp, qb::ImageOperationType_Clamp)
MAKE_IMAGE_NODE(ImageDot, qb::ImageOperationType_Dot)
MAKE_IMAGE_NODE(ImageCross, qb::ImageOperationType_Cross)
MAKE_IMAGE_NODE(ImageStep, qb::ImageOperationType_Step)
MAKE_IMAGE_NODE(ImagePow, qb::ImageOperationType_Pow)
MAKE_IMAGE_NODE(ImageSqrt, qb::ImageOperationType_Sqrt)
MAKE_IMAGE_NODE(ImageAbs, qb::ImageOperationType_Abs)
MAKE_IMAGE_NODE(ImageSin, qb::ImageOperationType_Sin)
MAKE_IMAGE_NODE(ImageCos, qb::ImageOperationType_Cos)
MAKE_IMAGE_NODE(ImageTan, qb::ImageOperationType_Tan)
MAKE_IMAGE_NODE(ImageMod, qb::ImageOperationType_Mod)
MAKE_IMAGE_NODE(ImageExp, qb::ImageOperationType_Exp)
MAKE_IMAGE_NODE(ImageLog, qb::ImageOperationType_Log)
MAKE_IMAGE_NODE(ImageMin, qb::ImageOperationType_Min)
MAKE_IMAGE_NODE(ImageMax, qb::ImageOperationType_Max)
MAKE_IMAGE_NODE(WhiteNoise, qb::ImageOperationType_WhiteNoise)
MAKE_IMAGE_NODE(ValueNoise, qb::ImageOperationType_ValueNoise)
MAKE_IMAGE_NODE(GradientNoise, qb::ImageOperationType_GradientNoise)
MAKE_IMAGE_NODE(SimplexNoise, qb::ImageOperationType_SimplexNoise)
MAKE_IMAGE_NODE(VoronoiNoise, qb::ImageOperationType_Voronoi)
MAKE_IMAGE_NODE(Mandelbrot, qb::ImageOperationType_Mandelbrot)
MAKE_IMAGE_NODE(Dynamics, qb::ImageOperationType_Dynamics)
MAKE_IMAGE_NODE(HighResOutput, qb::ImageOperationType_HighResOutput)
MAKE_IMAGE_NODE(UvDistortion, qb::ImageOperationType_UvDistortion)
MAKE_IMAGE_NODE(BlurFilter, qb::ImageOperationType_Blur)
MAKE_IMAGE_NODE(SharpenFilter, qb::ImageOperationType_Sharpen)
MAKE_IMAGE_NODE(MorphoFilter, qb::ImageOperationType_Morpho)
MAKE_IMAGE_NODE(BumpToNormal, qb::ImageOperationType_BumpToNormal)
MAKE_IMAGE_NODE(UvMapping, qb::ImageOperationType_UvMapping)
MAKE_IMAGE_NODE(UvInput, qb::ImageOperationType_UvInput)
MAKE_IMAGE_NODE(DirectionalSignal, qb::ImageOperationType_DirSignal)
MAKE_IMAGE_NODE(RadialSignal, qb::ImageOperationType_RadSignal)
MAKE_IMAGE_NODE(TimeInput, qb::ImageOperationType_Time)


#endif // TYPED_IMAGE_NODE_H
