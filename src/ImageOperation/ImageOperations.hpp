#ifndef QB_TEXTURE_OPERATIONS_HPP
#define QB_TEXTURE_OPERATIONS_HPP

#include "App/BaseOperationNode.hpp"

#include "App/AppInterface.hpp"
#include "ImageOperation/ImageAttributes.hpp"

#include "ImageOperation/GlslBuilder.hpp"

#include "Core/Vec2.h"


struct AbstractImageOperation;

struct TextureOperationInfo
{
    bool isSdf = false;
    BaseAttributes* attributes;
    AbstractImageOperation* operation;
    std::vector<TextureOperationInfo*> inputs;
};

using InputInfos = std::vector<TextureOperationInfo*>;

struct TextureOperationResult
{
    qb::GlslBuilderVisitor visitor;
    BaseAttributes* lastAttributes = nullptr;
    bool root = true;
};

struct AbstractImageOperation
{
    virtual ~AbstractImageOperation() = default;
    virtual void apply(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) = 0;
};

template<typename PropsType>
struct ConcretImageOperation : public AbstractImageOperation
{
    static constexpr size_t TypeId = PropsType::TypeId;

    void apply(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) override
    {
        return sample(result, static_cast<PropsType*>(attributes),inputs);
    }

    virtual void sample(TextureOperationResult& result, PropsType* attributes, InputInfos& inputs) = 0;
};

std::string pushOpOrInput(TextureOperationResult& result, TextureOperationInfo* input, const vec4 v);

//--------------------------------------------------------------
struct ColorInput : public ConcretImageOperation<ColorData>
{
    void sample(TextureOperationResult& result, ColorData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Construct3f : public ConcretImageOperation<Construct3fData>
{
    void sample(TextureOperationResult& result, Construct3fData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Split3f : public ConcretImageOperation<Split3fData>
{
    void sample(TextureOperationResult& result, Split3fData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Dynamics : public ConcretImageOperation<DynamicsData>
{
    void sample(TextureOperationResult& result, DynamicsData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct DirectionalSignal : public ConcretImageOperation<DirectionalSignalData>
{
    void sample(TextureOperationResult& result, DirectionalSignalData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct RadialSignal : public ConcretImageOperation<RadialSignalData>
{
    void sample(TextureOperationResult& result, RadialSignalData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct TimeInput : public ConcretImageOperation<TimeData>
{
    void sample(TextureOperationResult& result, TimeData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct HighResOutput : public ConcretImageOperation<ImageOutput>
{
    void sample(TextureOperationResult& result, ImageOutput* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct UvInput : public ConcretImageOperation<UvInputData>
{
    void sample(TextureOperationResult& result, UvInputData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct SphericalCoord : public ConcretImageOperation<SphericalCoordData>
{
    void sample(TextureOperationResult& result, SphericalCoordData* attributes, InputInfos& inputs) override;
    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct UvDistortion : public ConcretImageOperation<UvDistortionData>
{
    void sample(TextureOperationResult& result, UvDistortionData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct UvMapping : public ConcretImageOperation<UvMappingData>
{
    void sample(TextureOperationResult& result, UvMappingData* attributes, InputInfos& inputs) override;
    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct BlurFilter : public ConcretImageOperation<BlurFilterData>
{
    void sample(TextureOperationResult& result, BlurFilterData* attributes, InputInfos& inputs) override;
    void updateKernel(int radius);

    std::string getOperationCode() const;

    Kernel kernel;
};
//--------------------------------------------------------------
struct SharpenFilter : public ConcretImageOperation<SharpenFilterData>
{
    void sample(TextureOperationResult& result, SharpenFilterData* attributes, InputInfos& inputs) override;
    void updateKernel(int radius);

    std::string getOperationCode() const;

    Kernel kernel;
};
//--------------------------------------------------------------
struct MorphoFilter : public ConcretImageOperation<MorphoFilterData>
{
    void sample(TextureOperationResult& result, MorphoFilterData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct BumpToNormal : public ConcretImageOperation<BumpToNormalData>
{
    void sample(TextureOperationResult& result, BumpToNormalData* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct WhiteNoise : public ConcretImageOperation<WhiteNoiseData>
{
    void sample(TextureOperationResult& result, WhiteNoiseData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct ValueNoise : public ConcretImageOperation<ValueNoiseData>
{
    void sample(TextureOperationResult& result, ValueNoiseData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct GradientNoise : public ConcretImageOperation<GradientNoiseData>
{
    void sample(TextureOperationResult& result, GradientNoiseData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct SimplexNoise : public ConcretImageOperation<SimplexNoiseData>
{
    void sample(TextureOperationResult& result, SimplexNoiseData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct VoronoiNoise : public ConcretImageOperation<VoronoiNoiseData>
{
    void sample(TextureOperationResult& result, VoronoiNoiseData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Mandelbrot : public ConcretImageOperation<MandelbrotData>
{
    void sample(TextureOperationResult& result, MandelbrotData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

static std::unordered_map<qb::ImageOperationType, std::string> s_symbols = {
        {qb::ImageOperationType_Add, "+"},
        {qb::ImageOperationType_Sub, "-"},
        {qb::ImageOperationType_Mult, "*"},
        {qb::ImageOperationType_Div, "/"},
        {qb::ImageOperationType_Mix, "mix"},
        {qb::ImageOperationType_Clamp, "clamp"},
        {qb::ImageOperationType_Dot, "dot"},
        {qb::ImageOperationType_Cross, "cross"},
        {qb::ImageOperationType_Step, "step"},
        {qb::ImageOperationType_Sin, "sin"},
        {qb::ImageOperationType_Cos, "cos"},
        {qb::ImageOperationType_Tan, "tan"},
        {qb::ImageOperationType_Mod, "mod"},
        {qb::ImageOperationType_Pow, "pow"},
        {qb::ImageOperationType_Sqrt, "sqrt"},
        {qb::ImageOperationType_Abs, "abs"},
        {qb::ImageOperationType_Log, "log"},
        {qb::ImageOperationType_Exp, "exp"},
        {qb::ImageOperationType_Min, "min"},
        {qb::ImageOperationType_Max, "max"}
    };


//--------------------------------------------------------------
template<typename DataType>
struct ImageBasicOperator : public ConcretImageOperation<DataType>
{
    std::string operatorSymbol = s_symbols[(qb::ImageOperationType)DataType::TypeId];
    void sample(TextureOperationResult& result, DataType* attributes, InputInfos& inputs) override
    {
        auto in1 = attributes->in1;
        auto in2 = attributes->in2;

        auto& visitor = result.visitor;
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::string operand1 = pushOpOrInput(result, inputs[0], {in1,in1,in1,1.0f});
        std::string operand2 = pushOpOrInput(result, inputs[1], {in2,in2,in2,1.0f});

        size_t opId = context.getNextVa();
        std::string glsl = "vec4 $1 = vec4($2.xyz $3 $4.xyz, 1.0);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), operand1, operatorSymbol, operand2});
        context.pushVa(opId);
        context.pushCode(glsl);
    }
};

//--------------------------------------------------------------
#define MAKE_BASIC_OPERATOR(funcName) \
    using Image##funcName = ImageBasicOperator<Image##funcName##Data>;

//--------------------------------------------------------------
MAKE_BASIC_OPERATOR(Add)
MAKE_BASIC_OPERATOR(Sub)
MAKE_BASIC_OPERATOR(Mult)
MAKE_BASIC_OPERATOR(Div)

//--------------------------------------------------------------
template<typename DataType>
struct ImageBuiltInFunc : public ConcretImageOperation<DataType>
{
    std::string funcName = s_symbols[(qb::ImageOperationType)DataType::TypeId];
    std::string glslTemplate;

    void sample(TextureOperationResult& result, DataType* attributes, InputInfos& inputs) override
    {
        if (glslTemplate.empty())
        {
            glslTemplate = "vec4 $1 = vec4(" + funcName + "(";
            for(size_t i=0; i<attributes->argNames.size(); ++i)
            {
                if(i>0) glslTemplate += ",";
                glslTemplate += "$" + std::to_string(i+2) + ".xyz";
            }
            glslTemplate += "),1.0);\n";
        }

        auto& argValues = attributes->argValues;

        auto& visitor = result.visitor;
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::vector<std::string> idsCache(attributes->argNames.size() + 1);
        for(size_t i=0; i<argValues.size(); ++i)
        {
            float arg = argValues[i];
            idsCache[i+1] = pushOpOrInput(result, inputs[i],{arg,arg,arg,arg});
        }

        size_t opId = context.getNextVa();
        idsCache[0] = qb::va(opId);

        std::string glsl = qb::replaceArgs(glslTemplate, idsCache);
        context.pushVa(opId);
        context.pushCode(glsl);
    }
};

//--------------------------------------------------------------
#define MAKE_BUILTIN_OPERATION(funcName) \
    using Image##funcName = ImageBuiltInFunc<Image##funcName##Data>;

//--------------------------------------------------------------
MAKE_BUILTIN_OPERATION(Mix)
MAKE_BUILTIN_OPERATION(Clamp)
MAKE_BUILTIN_OPERATION(Dot)
MAKE_BUILTIN_OPERATION(Cross)
MAKE_BUILTIN_OPERATION(Step)
MAKE_BUILTIN_OPERATION(Sin)
MAKE_BUILTIN_OPERATION(Cos)
MAKE_BUILTIN_OPERATION(Tan)
MAKE_BUILTIN_OPERATION(Mod)
MAKE_BUILTIN_OPERATION(Pow)
MAKE_BUILTIN_OPERATION(Sqrt)
MAKE_BUILTIN_OPERATION(Abs)
MAKE_BUILTIN_OPERATION(Log)
MAKE_BUILTIN_OPERATION(Exp)
MAKE_BUILTIN_OPERATION(Min)
MAKE_BUILTIN_OPERATION(Max)


struct ProgramSet
{
    std::vector<size_t> ids;
    //std::vector<size_t> frames;
    std::vector<std::string> codes;

    ProgramSet(size_t c/*, int resolution*/)
    {
        ids.resize(c);
        // frames.resize(c);
        codes.resize(c);
        for(size_t i=0; i<c; ++i)
        {
            // frames[i] = RenderInterface::createTarget(resolution,resolution,false);
            ids[i] = RenderInterface::createCustomProgram();
        }
    }

    ~ProgramSet()
    {
        /*for(size_t id : frames)
            RenderInterface::deleteTarget((unsigned)id);*/
        for(size_t id : ids)
            RenderInterface::destroyCustomProgram((unsigned)id);
    }

    size_t at(size_t i) const
    {
        return ids[i];
    }

    /*size_t frameAt(size_t i) const
    {
        return frames[i];
    }*/

    const std::string& codeAt(size_t i) const
    {
        return codes[i];
    }

    void setCodeAt(size_t i, const std::string& code)
    {
        codes[i] = code;
    }
};

struct TexturePreview : BasePreview
{
    int resolution = 256;
    bool toRecompile = true;
    bool previewClicked = false;

    size_t glTextureId = 0;
    size_t glProgram = 0;
    std::string glslCode;

    std::unique_ptr<ProgramSet> programSet;

    TexturePreview(int resolution);

    ~TexturePreview();

    void attributePreview() override;

    void nodePreview(const Rect& previewArea) override;
};


#endif // QB_TEXTURE_OPERATIONS_HPP
