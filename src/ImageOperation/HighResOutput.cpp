#include "ImageOperation/HighResOutput.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
ColorInput::ColorInput()
    : ImageOperation(qb::ImageOperationType_Color)
{
    // makeInput("input", ImageDataType_Float);
    makeInput("red", ImageDataType_Float);
    makeInput("green", ImageDataType_Float);
    makeInput("blue", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("red", &r, 0.0f, 1.0f);
    makeProperty("green", &g, 0.0f, 1.0f);
    makeProperty("blue", &b, 0.0f, 1.0f);
    makeProperty("alpha", &a, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool ColorInput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;

    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({r,g,b,a});
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void ColorInput::uiProperties()
{
    uiPreview();
    vec4 color = {r,g,b,a};
    if (ImGui::ColorEdit4("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z; a = color.w;
        dirty(false);
    }
}

//--------------------------------------------------------------
ImageMult::ImageMult()
    : ImageOperation(qb::ImageOperationType_Mult)
{
    makeInput("input1", ImageDataType_Float);
    makeInput("input2", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool ImageMult::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();
    if(!sampleInput(1, t, visitor)) return false;
    size_t v2 = context.popVa();

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2 * $3;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1), qb::va(v2)});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}

//--------------------------------------------------------------
ImageAdd::ImageAdd()
    : ImageOperation(qb::ImageOperationType_Add)
{
    makeInput("input1", ImageDataType_Float);
    makeInput("input2", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool ImageAdd::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();
    if(!sampleInput(1, t, visitor)) return false;
    size_t v2 = context.popVa();

    size_t opId = context.getNextVa();
    
    std::string glsl = "vec4 $1 = $2 + $3;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1), qb::va(v2)});
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
ImageMix::ImageMix()
    : ImageOperation(qb::ImageOperationType_Mix)
{
    makeInput("input1", ImageDataType_Float);
    makeInput("input2", ImageDataType_Float);
    makeInput("delta", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("delta", &delta, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool ImageMix::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();
    if(!sampleInput(1, t, visitor)) return false;
    size_t v2 = context.popVa();

    bool inputValid =sampleInput(2, t, visitor);
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = mix($2, $3, $4);\n";
    std::vector<std::string> args;

    if(inputValid)
    {
        size_t v3 = context.popVa();
        args = {qb::va(opId), qb::va(v1), qb::va(v2), qb::va(v3)};
    }
    else
    {
        size_t v3 = frame.pushInput({delta,delta,delta,delta});
        args = {qb::va(opId), qb::va(v1), qb::va(v2), qb::in(v3)};
    }
    
    glsl = qb::replaceArgs(glsl, args);
    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
Dynamics::Dynamics()
    : ImageOperation(qb::ImageOperationType_Dynamics)
{
    makeInput("input1", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("brightness", &brightness, -1.0f, 1.0f);
    makeProperty("contrast", &contrast, 0.0f, 10.0f);
}
//--------------------------------------------------------------
bool Dynamics::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();
    
    if(sampleInput(0, t, visitor))
    {
        size_t v1 = context.popVa();
        size_t in1 = frame.pushInput({brightness,contrast,0.0f,0.0f});
        size_t opId = context.getNextVa();
        std::string glsl = "vec4 $1 = dynamics($2, $3);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1), qb::in(in1)});
        context.pushVa(opId);
        context.pushCode(glsl);
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    return false;
}
//--------------------------------------------------------------
std::string Dynamics::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 dynamics(vec4 color, vec4 bc){\n"
    "    vec3 c = (color.xyz - 0.5f) * bc.y + 0.5f + bc.x;\n"
    "    return vec4(c,1.0f);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
WhiteNoise::WhiteNoise()
    : ImageOperation(qb::ImageOperationType_WhiteNoise)
{
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool WhiteNoise::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
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
UvMap::UvMap()
    : ImageOperation(qb::ImageOperationType_UvMap)
{
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool UvMap::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;

    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4(uv0,0.0,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());
    
    return true;
}
//--------------------------------------------------------------
UvDistortion::UvDistortion()
    : ImageOperation(qb::ImageOperationType_UvDistortion)
{
    makeInput("input", ImageDataType_Float);
    makeInput("uv-map", ImageDataType_Float);
    makeOutput("output", ImageDataType_Float);
    makeProperty("u offset", &uOft, -1.0f, 1.0f);
    makeProperty("v offset", &vOft, -1.0f, 1.0f);
    makeProperty("u factor", &uFct, 0.0001f, 10.0f);
    makeProperty("v factor", &vFct, 0.0001f, 10.0f);
}
//--------------------------------------------------------------
bool UvDistortion::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t uvId2;
    std::string glsl;

    if(sampleInput(1, t, visitor)) // uv map
    {
        size_t v1 = context.popVa();
        
        uvId2 = context.getNextUv();
        size_t uvId = context.getUvId();
        glsl = "vec2 $1 = distortion($2,vec4($3.xy-uv0, 1.0,1.0));\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::va(v1)});
    }
    else
    {
        size_t in1 = frame.pushInput({uOft, vOft, uFct, vFct});

        uvId2 = context.getNextUv();
        size_t uvId = context.getUvId();
        
        glsl = "vec2 $1 = distortion($2,$3);\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::in(in1)});
    }
    
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    context.pushUv(uvId2);
    context.pushCode(glsl);
    bool ret = sampleInput(0, t, visitor);
    context.popUv();
    return ret;
}
//--------------------------------------------------------------
std::string UvDistortion::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 distortion(vec2 uv, vec4 oft){\n"
    "    return uv * oft.zw + oft.xy;\n"
    "}\n";
    return std::string(code);
}
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
    t.dstOp = this;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t ctx = frame.pushContext();
    bool inputValid = sampleInput(0, t, visitor);
    frame.popContext();

    if(inputValid)
    {
        size_t opId = context.getNextVa();
        size_t in1 = frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        size_t uvId = context.getUvId();

        std::string glsl =
        "vec4 v$1 = vec4(0.0);\n"
        "float s$1 = 1.0/256.0;\n"
        "int r$1 = int($2.x);\n"
        "for(int i=-r$1; i<=r$1; ++i)\n"
        "    for(int j=-r$1; j<=r$1; ++j)\n"
        "        v$1 += $3($4 + vec2(i,j)*s$1);\n"
        "float dv$1 = max(r$1*2.0+1.0, 3.0);\n"
        "dv$1 = dv$1 * dv$1;\n"
        "v$1 /= dv$1;\n";

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
    t.dstOp = this;
    
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
        "vec3 va$1 = normalize(vec3(2,0,a$1));\n"
        "vec3 vb$1 = normalize(vec3(0,2,b$1));\n"
        "v$1.xyz = cross(va$1,vb$1);\n";
        glsl = qb::replaceArgs(glsl, {std::to_string(opId), qb::fu(ctx), qb::uv(uvId)});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions(getNodeType(), getOperationCode());
        frame.hasUv = true;

        return true;
    }

    return false;
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
    t.dstOp = this;
    
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
HighResOutput::HighResOutput()
    : ImageOperation(qb::ImageOperationType_HighResOutput)
{
    makeInput("input", ImageDataType_Float);
    makeProperty("res", ImageDataType_Int, &res);
}
//--------------------------------------------------------------
bool HighResOutput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    size_t opId;
    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string glsl = "vec4 $1 = $2;\n";

    if(sampleInput(0, t, visitor))
    {
        size_t v1 = context.popVa();
        opId = context.getNextVa();
        glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1)});
    }
    else
    {
        size_t in1 = frame.pushInput({1.0f,1.0f,1.0f,1.0f});
        opId = context.getNextVa();
        glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::in(in1)});
    }
    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}