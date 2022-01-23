#ifndef QB_TEXTURE_OPERATIONS_HPP
#define QB_TEXTURE_OPERATIONS_HPP

#include "App/BaseOperationNode.hpp"
#include "App/AppInterface.hpp"
#include "ImageOperation/ImageAttributes.hpp"
#include "ImageOperation/GlslBuilder.hpp"
#include "ImageOperation/GlslProgramPipeline.hpp"

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
    virtual void buildProgram(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) = 0;
    virtual void setUniforms(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) = 0;
};

template<typename Type>
struct ImageOperation : public AbstractImageOperation
{
    using AttributeType = Type;
    static constexpr size_t TypeId = AttributeType::TypeId;

    void buildProgram(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) override
    {
        return buildProgramImpl(result, static_cast<AttributeType*>(attributes),inputs);
    }
    
    void setUniforms(TextureOperationResult& result, BaseAttributes* attributes, InputInfos& inputs) override
    {
        return setUniformsImpl(result, static_cast<AttributeType*>(attributes),inputs);
    }

    virtual void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) = 0;
    virtual void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) {};
};

// std::string pushOpOrInput(TextureOperationResult& result, TextureOperationInfo* input, const vec4 v);

//--------------------------------------------------------------
struct ColorInput : public ImageOperation<ColorData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Construct3f : public ImageOperation<Construct3fData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Split3f : public ImageOperation<Split3fData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct Dynamics : public ImageOperation<DynamicsData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct DirectionalSignal : public ImageOperation<DirectionalSignalData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct RadialSignal : public ImageOperation<RadialSignalData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct TimeInput : public ImageOperation<TimeData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct HighResOutput : public ImageOperation<ImageOutput>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct UvInput : public ImageOperation<UvInputData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct SphericalCoord : public ImageOperation<SphericalCoordData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct UvDistortion : public ImageOperation<UvDistortionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct UvMapping : public ImageOperation<UvMappingData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct BlurFilter : public ImageOperation<BlurFilterData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void updateKernel(int radius);

    std::string getOperationCode() const;

    Kernel kernel;
};
//--------------------------------------------------------------
struct SharpenFilter : public ImageOperation<SharpenFilterData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void updateKernel(int radius);

    std::string getOperationCode() const;

    Kernel kernel;
};
//--------------------------------------------------------------
struct MorphoFilter : public ImageOperation<MorphoFilterData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct BumpToNormal : public ImageOperation<BumpToNormalData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};
//--------------------------------------------------------------
struct WhiteNoise : public ImageOperation<WhiteNoiseData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct ValueNoise : public ImageOperation<ValueNoiseData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct GradientNoise : public ImageOperation<GradientNoiseData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct SimplexNoise : public ImageOperation<SimplexNoiseData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct VoronoiNoise : public ImageOperation<VoronoiNoiseData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Mandelbrot : public ImageOperation<MandelbrotData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

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
template<typename AttributeType>
struct ImageBasicOperator : public ImageOperation<AttributeType>
{
    std::string operatorSymbol = s_symbols[(qb::ImageOperationType)AttributeType::TypeId];
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override
    {
        auto in1 = attributes->in1;
        auto in2 = attributes->in2;

        auto& visitor = result.visitor;
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::string operand1 = visitor.inputOrUniformPlaceholder(result, inputs[0]);
        std::string operand2 = visitor.inputOrUniformPlaceholder(result, inputs[1]);

        size_t opId = context.getNextVa();
        std::string glsl = "vec4 $1 = vec4($2.xyz $3 $4.xyz, 1.0);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), operand1, operatorSymbol, operand2});
        context.pushVa(opId);
        context.pushCode(glsl);
    }
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override
    {
        auto in1 = attributes->in1;
        auto in2 = attributes->in2;

        auto& visitor = result.visitor;
        visitor.inputOrUniform(result, inputs[0], {in1,in1,in1,1.0f});
        visitor.inputOrUniform(result, inputs[1], {in2,in2,in2,1.0f});
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
template<typename AttributeType>
struct ImageBuiltInFunc : public ImageOperation<AttributeType>
{
    std::string funcName = s_symbols[(qb::ImageOperationType)AttributeType::TypeId];
    std::string glslTemplate;

    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override
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
            idsCache[i+1] = visitor.inputOrUniformPlaceholder(result, inputs[i]);
        }

        size_t opId = context.getNextVa();
        idsCache[0] = qb::va(opId);

        std::string glsl = qb::replaceArgs(glslTemplate, idsCache);
        context.pushVa(opId);
        context.pushCode(glsl);
    }

    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override
    {
        size_t i=0;
        for(auto arg : attributes->argValues)
            result.visitor.inputOrUniform(result, inputs[i++],{arg,arg,arg,arg});
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
    std::vector<std::string> codes;

    ProgramSet(size_t c)
    {
        ids.resize(c);
        codes.resize(c);
        for(size_t i=0; i<c; ++i)
            ids[i] = RenderInterface::createCustomProgram();
    }

    ~ProgramSet()
    {
        for(size_t id : ids)
            RenderInterface::destroyCustomProgram((unsigned)id);
    }

    size_t at(size_t i) const
    {
        return ids[i];
    }

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
    std::unique_ptr<qb::GlslProgramPipeline> programPipeline;
    std::unique_ptr<qb::GlslPipelineData> uniforms;

    TexturePreview(int resolution);

    ~TexturePreview();

    void attributePreview() override;

    void nodePreview(const Rect& previewArea) override;
};


#endif // QB_TEXTURE_OPERATIONS_HPP
