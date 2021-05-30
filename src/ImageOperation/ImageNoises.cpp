#include "ImageOperation/ImageNoises.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
WhiteNoise::WhiteNoise()
    : ImageOperation(qb::ImageOperationType_WhiteNoise)
{
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool WhiteNoise::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    std::string glsl = "vec4 $1 = white_noise($2);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
std::string WhiteNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_wnoise(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 white_noise(vec2 uv){\n"
    "    float r = rand_wnoise(uv);\n"
    "    return vec4(r,r,r,1.0f);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
PerlinNoise::PerlinNoise()
    : ImageOperation(qb::ImageOperationType_Perlin)
{
    makeOutput("output", ImageDataType_Float);
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("smoothness", &smoothness, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool PerlinNoise::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    size_t opId = context.getNextVa();
    size_t in1 = frame.pushInput({(float)octaves, frequency, persistance, smoothness});
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = perlin($2, $3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(in1), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}

//--------------------------------------------------------------
std::string PerlinNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_perlin(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 perlin(vec4 res, vec2 uv){\n"
    "    int octaves = int(res.x);\n"
    "    vec2 f = res.yy;\n"
    "    float per = res.z;\n"
    "    float ret = 0.0f;\n"
    "    float w = 0.0f;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0f, 1.0f);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fuv = fract(uv * f);\n"
    "        fuv = res.w * fuv * fuv * (3.f - 2.f * fuv);\n"
    "        float s1 = rand_perlin(iuv + oft.xx);\n"
    "        float s2 = rand_perlin(iuv + oft.yx);\n"
    "        float s3 = rand_perlin(iuv + oft.xy);\n"
    "        float s4 = rand_perlin(iuv + oft.yy);\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        w += per;\n"
    "        f *= 2.0f;\n"
    "        per *= res.z;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1f, w);\n"
    "    return vec4(ret,ret,ret,1.0f);\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
VoronoiNoise::VoronoiNoise()
    : ImageOperation(qb::ImageOperationType_Voronoi)
{
    makeOutput("output", ImageDataType_Float);
    makeProperty("octaves", &octaves, 1, 10);
    makeProperty("frequency", &frequency, 1.0f, 32.0f);
    makeProperty("persistance", &persistance, 0.0f, 1.0f);
    makeProperty("smoothness", &smoothness, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool VoronoiNoise::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    size_t opId = context.getNextVa();
    size_t in1 = frame.pushInput({(float)octaves, frequency, persistance, smoothness});
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = voronoi($2, $3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(in1), qb::uv(uvId)});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}

//--------------------------------------------------------------
std::string VoronoiNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_voronoi(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 voronoi(vec4 res, vec2 uv){\n"
    "    int octaves = int(res.x);\n"
    "    vec2 f = res.yy;\n"
    "    float per = res.z;\n"
    "    float ret = 0.0f;\n"
    "    float w = 0.0f;\n"
    "    for(int i=0; i<10; ++i){\n"
    "        vec2 oft = vec2(0.0f, 1.0f);\n"
    "        vec2 iuv = floor(uv * f);\n"
    "        vec2 fuv = fract(uv * f);\n"
    "        fuv = res.w * fuv * fuv * (3.f - 2.f * fuv);\n"
    "        float s1 = rand_voronoi(iuv + oft.xx);\n"
    "        float s2 = rand_voronoi(iuv + oft.yx);\n"
    "        float s3 = rand_voronoi(iuv + oft.xy);\n"
    "        float s4 = rand_voronoi(iuv + oft.yy);\n"
    "        float s12 = mix(s1,s2,fuv.x);\n"
    "        float s34 = mix(s3,s4,fuv.x);\n"
    "        float s1234 = mix(s12,s34,fuv.y);\n"
    "        ret += s1234 * per;\n"
    "        w += per;\n"
    "        f *= 2.0f;\n"
    "        per *= res.z;\n"
    "        if (i>=octaves-1) break;\n"
    "    }\n"
    "    ret = ret / max(0.1f, w);\n"
    "    return vec4(ret,ret,ret,1.0f);\n"
    "}\n";
    return std::string(code);
}