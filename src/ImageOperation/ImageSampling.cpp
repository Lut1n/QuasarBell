#include "ImageOperation/ImageSampling.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
ImageFilter::ImageFilter(qb::ImageOperationType type)
    : ImageOperation(type)
    , kernel({1})
{
    makeInput("in", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
bool ImageFilter::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    updateKernel();

    auto& frame = visitor.getCurrentFrame();

    size_t inputFrame = 0;
    visitor.pushFrame();
    bool inputValid = sampleInput(0, visitor);
    visitor.popFrame();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t ke1 = frame.pushKernel(kernel);
        size_t uvId = context.getUvId();

        std::string glsl = "vec4 v$1 = image_filter($3, $4, $5, int($2.x));\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(inputFrame), qb::uv(uvId), qb::ke(ke1)});
        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions(getNodeType(), getOperationCode());
        frame.hasUv = true;

        return true;
    }

    return false;
}
//--------------------------------------------------------------
std::string ImageFilter::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 image_filter(sampler2D src, vec2 uv, const in float kernel[512], int radius){\n"
    "    vec4 ret = vec4(vec3(0.0),1.0);\n"
    "    float ps = 1.0/resolution;\n"
    "    for(int i=-radius; i<=radius; ++i){\n"
    "    for(int j=-radius; j<=radius; ++j){\n"
    "        vec2 kuv = uv + vec2(i,j)*ps;\n"
    "        ret.xyz += vec3(kernel[(i+radius)*(radius*2+1)+(j+radius)]) * texture2D(src,kuv).xyz;\n"
    "    }}\n"
    "    return ret;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void ImageFilter::updateKernel()
{
    size_t size = radius*2+1;
    kernel = Kernel({1}).resize(size,size);
}

//--------------------------------------------------------------
BlurFilter::BlurFilter()
    : ImageFilter(qb::ImageOperationType_Blur)
{
}
//--------------------------------------------------------------
void BlurFilter::updateKernel()
{
    // gaussian
    Kernel k0(3,3);
    for(int i=0;i<9;++i)k0.data[i] = (float)(1.0/9.0);
    kernel = k0;
    for(int i=1; i<radius; ++i)
        kernel = Kernel::convProduct(kernel, k0);
}

//--------------------------------------------------------------
SharpenFilter::SharpenFilter()
    : ImageFilter(qb::ImageOperationType_Sharpen)
{
}
//--------------------------------------------------------------
void SharpenFilter::updateKernel()
{
    Kernel k0(3,3);
    k0 = { 0,-1, 0,
          -1, 5,-1,
           0,-1, 0};
    kernel = k0;
    for(int i=1; i<radius; ++i)
        kernel = Kernel::convProduct(kernel, k0);
}

//--------------------------------------------------------------
MorphoFilter::MorphoFilter()
    : ImageOperation(qb::ImageOperationType_Morpho)
{
    makeInput("in", BaseOperationDataType::Float);
    makeInput("radius", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("radius", &radius, 1, 10);
    makeProperty("mode", &mode, 0, 1);
}
//--------------------------------------------------------------
bool MorphoFilter::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();

    size_t inputFrame = 0;
    visitor.pushFrame();
    bool inputValid = sampleInput(0, visitor);
    visitor.popFrame();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        float radiusf = (float)radius;
        std::string radiusId = pushOpOrInput(1,visitor, {radiusf,radiusf,radiusf,radiusf});

        size_t in1 = frame.pushInput({(float)mode, 0.0, 0.0, 0.0});

        size_t opId = context.getNextVa();
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(vec3(1.0-$2.x),1.0);\n"
        "float s$1 = 1.0/resolution;\n"
        "int r$1 = int($5.x);\n"
        "for(int i=-r$1; i<=r$1; ++i){\n"
        "for(int j=-r$1; j<=r$1; ++j){\n"
        "    if(i*i + j*j > r$1*r$1) continue;\n"
        "    vec2 kuv$1 = $4 + vec2(i,j)*s$1;\n"
        "    if ($2.x == 0.0)\n"
        "        v$1.xyz = min(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "    else\n"
        "        v$1.xyz = max(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "}}\n";

        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(inputFrame), qb::uv(uvId), radiusId});
        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions(getNodeType(), getOperationCode());
        frame.hasUv = true;

        return true;
    }

    return false;
}
//--------------------------------------------------------------
BumpToNormal::BumpToNormal()
    : ImageOperation(qb::ImageOperationType_BumpToNormal)
{
    makeInput("bump", BaseOperationDataType::Float);
    makeOutput("normal", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool BumpToNormal::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t ctx = frame.pushContext();
    bool inputValid = sampleInput(0, visitor);
    frame.popContext();

    if(inputValid)
    {
        size_t opId = context.getNextVa();
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(0,0,0,1);\n"
        "float s$1 = 1.0/resolution;\n"
        "float a$1 = $2($3 + vec2(-1,0)*s$1).x - $2($3 + vec2(1,0)*s$1).x;\n"
        "float b$1 = $2($3 + vec2(0,-1)*s$1).x - $2($3 + vec2(0,1)*s$1).x;\n"
        "vec3 va$1 = normalize(vec3(2,0,a$1 * resolution));\n"
        "vec3 vb$1 = normalize(vec3(0,2,b$1 * resolution));\n"
        "v$1.xyz = cross(va$1,vb$1) * 0.5 + 0.5;\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::fu(ctx), qb::uv(uvId)});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions(getNodeType(), getOperationCode());
        frame.hasUv = true;

        return true;
    }

    return false;
}