#include "ImageOperation/ImageSampling.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
BlurFilter::BlurFilter()
    : ImageOperation(qb::ImageOperationType_BlurFilter)
{
    makeInput("input", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("radius", &radius, 1, 10);
}
//--------------------------------------------------------------
bool BlurFilter::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();

    size_t ctx = frame.pushContext();
    bool inputValid = sampleInput(0, t, visitor);
    frame.popContext();

    if(inputValid)
    {
        auto& context = frame.getContext();
        
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(0.0);\n"
        "float s$1 = 1.0/256.0;\n"
        "int r$1 = int($2.x);\n"
        "int d$1 = r$1/2;\n"
        "int c$1 = r$1 * r$1;\n"
        "int i$1=-d$1,j$1=-d$1;\n"
        "for(int k=0; k<c$1; ++k){\n"
        "    v$1 += $3($4 + vec2(i$1,j$1)*s$1);\n"
        "    j$1++;\n"
        "    if(j$1>d$1) { i$1++; j$1=-d$1; }\n"
        "}\n"
        "v$1 /= c$1;\n";

        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::in(in1), qb::fu(ctx), qb::uv(uvId)});
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
        "float s$1 = 1.0/256.0;\n"
        "float a$1 = $2($3 + vec2(-1,0)*s$1).x - $2($3 + vec2(1,0)*s$1).x;\n"
        "float b$1 = $2($3 + vec2(0,-1)*s$1).x - $2($3 + vec2(0,1)*s$1).x;\n"
        "vec3 va$1 = normalize(vec3(2,0,a$1 * 256.0));\n"
        "vec3 vb$1 = normalize(vec3(0,2,b$1 * 256.0));\n"
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