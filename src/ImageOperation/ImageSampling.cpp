#include "ImageOperation/ImageSampling.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
BlurFilter::BlurFilter()
    : ImageOperation(qb::ImageOperationType_Blur)
{
    makeInput("input", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
bool BlurFilter::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    /*float to = radius*2+1;
    Kernel k(to,to);
    float bv = 1.0/(to*to);
    for(int i = 0; i<k.w;++i)
    for(int j = 0; j<k.h;++j)
        k(i,j) = bv;*/

    // gaussian
    Kernel k0(3,3);
    for(int i=0;i<9;++i)k0.data[i] = 1.0/9.0;
    Kernel k = k0;
    for(int i=1; i<radius; ++i)
        k = Kernel::convProduct(k, k0);

    // Kernel::display(k);

    auto& frame = visitor.getCurrentFrame();

    size_t inputFrame = 0;
    visitor.pushFrame();
    bool inputValid = sampleInput(0, t, visitor);
    visitor.popFrame();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t ke1 = frame.pushKernel(k);
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(vec3(0.0),1.0);\n"
        "float s$1 = 1.0/resolution;\n"
        "int r$1 = int($2.x);\n"
        "float c$1 = (r$1*2.0+1.0);\n"
        "c$1 = c$1*c$1;\n"
        "for(int i=-r$1; i<=r$1; ++i){\n"
        "for(int j=-r$1; j<=r$1; ++j){\n"
        "    vec2 kuv$1 = $4 + vec2(i,j)*s$1;\n"
        "    kuv$1.y = 1.0 - kuv$1.y;\n"
        "    v$1.xyz += vec3($5[(i+r$1)*(r$1*2+1)+(j+r$1)]) * texture2D($3,kuv$1).xyz;\n"
        "}}\n";
        //"v$1.xyz /= c$1;\n";

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
SharpenFilter::SharpenFilter()
    : ImageOperation(qb::ImageOperationType_Sharpen)
{
    makeInput("input", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
bool SharpenFilter::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    Kernel k0(3,3);
    k0 = { 0,-1, 0,
          -1, 5,-1,
           0,-1, 0};
    Kernel k = k0;
    for(int i=1; i<radius; ++i)
        k = Kernel::convProduct(k, k0);

    auto& frame = visitor.getCurrentFrame();

    size_t inputFrame = 0;
    visitor.pushFrame();
    bool inputValid = sampleInput(0, t, visitor);
    visitor.popFrame();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t ke1 = frame.pushKernel(k);
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(vec3(0.0),1.0);\n"
        "float s$1 = 1.0/resolution;\n"
        "int r$1 = int($2.x);\n"
        "float c$1 = (r$1*2.0+1.0);\n"
        "c$1 = c$1*c$1;\n"
        "for(int i=-r$1; i<=r$1; ++i){\n"
        "for(int j=-r$1; j<=r$1; ++j){\n"
        "    vec2 kuv$1 = $4 + vec2(i,j)*s$1;\n"
        "    kuv$1.y = 1.0 - kuv$1.y;\n"
        "    v$1.xyz += vec3($5[(i+r$1)*(r$1*2+1)+(j+r$1)]) * texture2D($3,kuv$1).xyz;\n"
        "}}\n";

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
MorphoFilter::MorphoFilter()
    : ImageOperation(qb::ImageOperationType_Morpho)
{
    makeInput("input", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("radius", &radius, 1, 10);
    makeProperty("mode", &mode, 0, 1);
}
//--------------------------------------------------------------
bool MorphoFilter::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();

    size_t inputFrame = 0;
    visitor.pushFrame();
    bool inputValid = sampleInput(0, t, visitor);
    visitor.popFrame();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, (float)mode, 0.0, 0.0});
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(vec3(1.0-$2.y),1.0);\n"
        "float s$1 = 1.0/resolution;\n"
        "int r$1 = int($2.x);\n"
        "for(int i=-r$1; i<=r$1; ++i){\n"
        "for(int j=-r$1; j<=r$1; ++j){\n"
        "    if(i*i + j*j > r$1*r$1) continue;\n"
        "    vec2 kuv$1 = $4 + vec2(i,j)*s$1;\n"
        "    kuv$1.y = 1.0 - kuv$1.y;\n"
        "    if ($2.y == 0.0)\n"
        "        v$1.xyz = min(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "    else\n"
        "        v$1.xyz = max(v$1.xyz, texture2D($3,kuv$1).xyz);\n"
        "}}\n";

        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::sa(inputFrame), qb::uv(uvId)});
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
    makeInput("bump", ImageDataType_Float);
    makeOutput("normal", ImageDataType_Float);
}
//--------------------------------------------------------------
bool BumpToNormal::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t ctx = frame.pushContext();
    bool inputValid = sampleInput(0, t, visitor);
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