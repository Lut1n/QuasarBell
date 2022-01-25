#ifndef QB_IMAGE_ATTRIBUTES_HPP
#define QB_IMAGE_ATTRIBUTES_HPP

#include "ImageOperation/ImageOperationType.hpp"
#include "App/BaseAttributes.hpp"
#include "Core/Vec2.h"


//--------------------------------------------------------------
struct ColorData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Color;

    ColorData();

    void ui() override;

    float r=1.0f,g=1.0f,b=1.0f,a=1.0f;
};
//--------------------------------------------------------------
struct Construct3fData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Construct3f;

    Construct3fData();

    float c1=0.0f,c2=0.0f,c3=0.0f;
};
//--------------------------------------------------------------
struct Split3fData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Split3f;

    Split3fData();

    void ui() override;

    float r=1.0f,g=1.0f,b=1.0f;
    int index = 0;
};
//--------------------------------------------------------------
struct DynamicsData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Dynamics;

    DynamicsData();

    float brightness = 0.0f;
    float contrast = 1.0f;
};
//--------------------------------------------------------------
struct DirectionalSignalData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_DirSignal;

    DirectionalSignalData();

    float directionX = 1.0f;
    float directionY = 1.0f;
    float frequency = 4.0f;
    float amplitude = 1.0f;
};
//--------------------------------------------------------------
struct RadialSignalData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_RadSignal;

    RadialSignalData();

    float centerX = 1.0f;
    float centerY = 1.0f;
    float frequency = 4.0f;
    float amplitude = 1.0f;
};
//--------------------------------------------------------------
struct TimeData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Time;

    TimeData();
};
//--------------------------------------------------------------
struct ImageOutput : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_HighResOutput;

    ImageOutput();

    void ui() override;

    int resolution = 8;
    int textureResolution = 256;

    std::string path = "exported/output.tga";
    bool toExport = false;

};
//--------------------------------------------------------------
struct UvInputData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_UvInput;

    UvInputData();
};
//--------------------------------------------------------------
struct SphericalCoordData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_SphericalCoord;

    SphericalCoordData();
};
//--------------------------------------------------------------
struct UvDistortionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_UvDistortion;

    UvDistortionData();

    float force = 0.1f;
};
//--------------------------------------------------------------
struct UvMappingData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_UvMapping;

    UvMappingData();

    float uOft = 0.0f;
    float vOft = 0.0f;
    float uFct = 1.0f;
    float vFct = 1.0f;
};
//--------------------------------------------------------------
struct BlurFilterData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Blur;

    BlurFilterData();
    
    void updateKernel();
    Kernel kernel;

    int radius = 1;
};
//--------------------------------------------------------------
struct SharpenFilterData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Sharpen;

    SharpenFilterData();

    void updateKernel();
    Kernel kernel;

    int radius = 1;
};
//--------------------------------------------------------------
struct MorphoFilterData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Morpho;

    MorphoFilterData();

    int radius = 1;
    int mode = 0;
};
//--------------------------------------------------------------
struct BumpToNormalData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_BumpToNormal;

    BumpToNormalData();
};
//--------------------------------------------------------------
struct WhiteNoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_WhiteNoise;

    WhiteNoiseData();
};
//--------------------------------------------------------------
struct ValueNoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_ValueNoise;

    ValueNoiseData();

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};
//--------------------------------------------------------------
struct GradientNoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_GradientNoise;

    GradientNoiseData();

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};
//--------------------------------------------------------------
struct SimplexNoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_SimplexNoise;

    SimplexNoiseData();

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};
//--------------------------------------------------------------
struct VoronoiNoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Voronoi;

    VoronoiNoiseData();

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    int mode = 0;
};
//--------------------------------------------------------------
struct MandelbrotData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::ImageOperationType_Mandelbrot;

    MandelbrotData();

    int iterations = 20;
    float oftx = 1.7f;
    float ofty = 1.0f;
    float scale = 2.0f;
};




//--------------------------------------------------------------
template<size_t BasicTypeId>
struct ImageBasicOperatorData : public BaseAttributes
{
    static constexpr size_t TypeId = BasicTypeId;

    ImageBasicOperatorData()
        : BaseAttributes(TypeId)
    {
        add("in1", Type::Float, &in1);
        add("in2", Type::Float, &in2);
        addInput("in1", IoType::Image);
        addInput("in2", IoType::Image);
        addOutput("out", IoType::Image);
    }

    float in1 = 0.0f;
    float in2 = 0.0f;
};

//--------------------------------------------------------------
#define MAKE_BASIC_OPERATOR_DATA(funcName, Id) \
    using Image##funcName##Data = ImageBasicOperatorData<Id>;

//--------------------------------------------------------------
MAKE_BASIC_OPERATOR_DATA(Add, (size_t)qb::ImageOperationType_Add)
MAKE_BASIC_OPERATOR_DATA(Sub, (size_t)qb::ImageOperationType_Sub)
MAKE_BASIC_OPERATOR_DATA(Mult, (size_t)qb::ImageOperationType_Mult)
MAKE_BASIC_OPERATOR_DATA(Div, (size_t)qb::ImageOperationType_Div)


//--------------------------------------------------------------
template<size_t BasicTypeId>
struct ImageBuiltInFuncData : public BaseAttributes
{
    static constexpr size_t TypeId = BasicTypeId;

    ImageBuiltInFuncData(const std::initializer_list<std::string>& names)
        : BaseAttributes(TypeId)
    {
        argNames = names;
        argValues.resize(argNames.size());

        for(size_t i=0; i<argNames.size(); ++i)
        {
            auto& name = argNames[i];
            addInput(name, IoType::Image);
            add(name, Type::Float, &argValues[i]);
        }
        addOutput("out", IoType::Image);
    }

    std::vector<std::string> argNames;
    std::vector<float> argValues;
};

//--------------------------------------------------------------
#define MAKE_BUILTIN_OPERATION_DATA(funcName, Id, Names) \
    struct Image##funcName##Data : public ImageBuiltInFuncData<Id>{\
        Image##funcName##Data() : ImageBuiltInFuncData(Names) {}\
    };

#define QB_ARGS(...) __VA_ARGS__

//--------------------------------------------------------------
MAKE_BUILTIN_OPERATION_DATA(Mix, (size_t)qb::ImageOperationType_Mix, QB_ARGS({"in1", "in2", "delta"}))
MAKE_BUILTIN_OPERATION_DATA(Clamp, (size_t)qb::ImageOperationType_Clamp, QB_ARGS({"in1", "edge0", "edge1"}))
MAKE_BUILTIN_OPERATION_DATA(Dot, (size_t)qb::ImageOperationType_Dot, QB_ARGS({"in1", "in2"}))
MAKE_BUILTIN_OPERATION_DATA(Cross, (size_t)qb::ImageOperationType_Cross, QB_ARGS({"in1", "in2"}))
MAKE_BUILTIN_OPERATION_DATA(Step, (size_t)qb::ImageOperationType_Step, QB_ARGS({"edge", "in"}))
MAKE_BUILTIN_OPERATION_DATA(Sin, (size_t)qb::ImageOperationType_Sin, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Cos, (size_t)qb::ImageOperationType_Cos, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Tan, (size_t)qb::ImageOperationType_Tan, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Mod, (size_t)qb::ImageOperationType_Mod, QB_ARGS({"in", "mod"}))
MAKE_BUILTIN_OPERATION_DATA(Pow, (size_t)qb::ImageOperationType_Pow, QB_ARGS({"in", "pow"}))
MAKE_BUILTIN_OPERATION_DATA(Sqrt, (size_t)qb::ImageOperationType_Sqrt, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Abs, (size_t)qb::ImageOperationType_Abs, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Log, (size_t)qb::ImageOperationType_Log, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Exp, (size_t)qb::ImageOperationType_Exp, QB_ARGS({"in"}))
MAKE_BUILTIN_OPERATION_DATA(Min, (size_t)qb::ImageOperationType_Min, QB_ARGS({"in1", "in2"}))
MAKE_BUILTIN_OPERATION_DATA(Max, (size_t)qb::ImageOperationType_Max, QB_ARGS({"in1", "in2"}))


#endif // QB_IMAGE_ATTRIBUTES_HPP
