#include "ImageOperation/HighResOutput.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"


namespace qb
{
    std::string varName(size_t i)
    {
        return std::string("v") + std::to_string(i);
    }
    std::string uvName(size_t i)
    {
        return std::string("uv") + std::to_string(i);
    }
    std::string ctxtName(size_t i)
    {
        return std::string("context_") + std::to_string(i);
    }

    std::string inputName(size_t i)
    {
        return std::string("u") + std::to_string(i);
    }

    std::string opEnd()
    {
        return std::string(";\n");
    }

    std::string funCall(const std::string& name, const std::vector<std::string>& args)
    {
        std::string ret = name + std::string("(");
        bool first = true;
        for(auto& arg : args)
        {
            if (!first) ret += std::string(",");
            else first = false;
            ret += arg;
        }
        ret += std::string(")");
        return ret;
    }

    std::string varNew(size_t i)
    {
        return std::string("vec4 ") + varName(i) + std::string(" = ");
    }
    std::string uvNew(size_t i)
    {
        return std::string("vec2 ") + uvName(i) + std::string(" = ");
    }
};

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
bool ColorInput::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t input_id = data.global.pushUniform({r,g,b,a});
    size_t op_id = data.getContext().getNextOperationId();

    std::string opcode = qb::varNew(op_id) + qb::inputName(input_id) + qb::opEnd();

    data.getContext().pushOperation(op_id, opcode);
    addOperationCode(data);

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
bool ImageMult::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;
    
    if(!sampleInput(0, t, data)) return false;
    size_t var1 = data.getContext().popResultId();
    if(!sampleInput(1, t, data)) return false;
    size_t var2 = data.getContext().popResultId();

    size_t op_id = data.getContext().getNextOperationId();
    std::string opcode = qb::varNew(op_id) + qb::varName(var1) + std::string(" * ") + qb::varName(var2) + qb::opEnd();
    data.getContext().pushOperation(op_id, opcode);
    addOperationCode(data);
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
bool ImageAdd::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;
    
    if(!sampleInput(0, t, data)) return false;
    size_t var1 = data.getContext().popResultId();
    if(!sampleInput(1, t, data)) return false;
    size_t var2 = data.getContext().popResultId();

    size_t op_id = data.getContext().getNextOperationId();
    std::string opcode = qb::varNew(op_id) + qb::varName(var1) + std::string(" + ") + qb::varName(var2) + qb::opEnd();
    data.getContext().pushOperation(op_id, opcode);
    addOperationCode(data);
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
bool ImageMix::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    if(!sampleInput(0, t, data)) return false;
    size_t var1 = data.getContext().popResultId();
    if(!sampleInput(1, t, data)) return false;
    size_t var2 = data.getContext().popResultId();

    std::vector<std::string> args;
    if (sampleInput(2, t, data))
    {
        size_t var3 = data.getContext().popResultId();
        args = {qb::varName(var1), qb::varName(var2), qb::varName(var3)};
    }
    else
    {
        std::string vec4str = std::string("vec4(") + std::to_string(delta) + std::string(")");
        args = {qb::varName(var1), qb::varName(var2), vec4str};
    }
    
    size_t op_id = data.getContext().getNextOperationId();
    std::string opcode = qb::varNew(op_id) + qb::funCall("mix", args) + qb::opEnd();
    data.getContext().pushOperation(op_id, opcode);
    addOperationCode(data);
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
bool Dynamics::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;
    
    if(sampleInput(0, t, data))
    {
        size_t var1 = data.getContext().popResultId();
        size_t input_id = data.global.pushUniform({brightness,contrast,0.0f,0.0f});
        size_t op_id = data.getContext().getNextOperationId();
        std::string opcode = qb::varNew(op_id) + qb::funCall("dynamics", {qb::varName(var1), qb::inputName(input_id)}) + qb::opEnd();
        data.getContext().pushOperation(op_id, opcode);
        addOperationCode(data);
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
bool WhiteNoise::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t op_id = data.getContext().getNextOperationId();
    size_t uvId = data.getContext().getUvId();
    std::string opcode = qb::varNew(op_id) + qb::funCall("white_noise", {qb::uvName(uvId)}) + qb::opEnd();

    data.getContext().pushOperation(op_id, opcode);
    data.global.useUV = true;

    addOperationCode(data);

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
bool UvMap::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t op_id = data.getContext().getNextOperationId();
    std::string opcode = qb::varNew(op_id) + std::string("vec4(uv0,0.0f,1.0f)") + qb::opEnd();

    data.getContext().pushOperation(op_id, opcode);
    data.global.useUV = true;

    addOperationCode(data);
    
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
bool UvDistortion::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t uv_id;
    std::string opcode;

    if(sampleInput(1, t, data)) // uv map
    {
        size_t var1 = data.getContext().popResultId();
        
        uv_id = data.getContext().getNextUvId();
        size_t uvId = data.getContext().getUvId();
        opcode = qb::uvNew(uv_id) + qb::funCall("distortion", {qb::uvName(uvId), std::string("vec4(") + qb::varName(var1) + std::string(".xy") + std::string("-uv0, 1.0f, 1.0f)")}) + qb::opEnd();
    }
    else
    {
        size_t input_id = data.global.pushUniform({uOft, vOft, uFct, vFct});

        uv_id = data.getContext().getNextUvId();
        size_t uvId = data.getContext().getUvId();
        opcode = qb::uvNew(uv_id) + qb::funCall("distortion", {qb::uvName(uvId), qb::inputName(input_id)}) + qb::opEnd();
    }
    
    addOperationCode(data);
    data.global.useUV = true;

    data.getContext().pushUv(uv_id, opcode);
    bool ret = sampleInput(0, t, data);
    data.getContext().popUv();
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
bool BlurFilter::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t context_id = data.pushContext();
    bool inputValid = sampleInput(0, t, data);
    data.popContext();

    std::string contextName = qb::ctxtName(context_id);

    if(inputValid)
    {
        size_t op_id = data.getContext().getNextOperationId();
        size_t input_id = data.global.pushUniform({(float)radius, 0.0, 0.0, 0.0});
        size_t uvId = data.getContext().getUvId();

        std::string rid = std::string("r") + std::to_string(op_id);
        std::string dvname = std::string("dv") + std::to_string(op_id);
        std::string code;
        code += std::string("    float s") + std::to_string(op_id) + std::string(" = 1.0f/256.0f;\n");
        code += std::string("    int ") + rid + std::string(" = int(") + qb::inputName(input_id) + std::string(".x)") + qb::opEnd();
        code += std::string("    for(int i=-") + rid + std::string("; i<=") + rid + std::string("; ++i)\n");
        code += std::string("    for(int j=-") + rid + std::string("; j<=") + rid + std::string("; ++j)\n");

        std::string opcode = qb::varNew(op_id) + std::string("vec4(0.0,0.0,0.0,0.0)") + qb::opEnd();
        opcode += code;
        opcode +=  std::string("        ") + qb::varName(op_id) + std::string(" += ") + qb::funCall(contextName, {qb::uvName(uvId) + std::string(" + vec2(i,j)*s") + std::to_string(op_id)}) + qb::opEnd();
        opcode += std::string("    ") + std::string("float ") + dvname + std::string(" = max(") + rid + std::string("*2.0f+1.0f, 3.0f)") + qb::opEnd(); 
        opcode += std::string("    ") + dvname + std::string(" = ") + dvname + std::string(" * ") + dvname + qb::opEnd(); 
        opcode += std::string("    ") + qb::varName(op_id) + std::string("/=") + dvname + qb::opEnd();

        data.getContext().pushOperation(op_id, opcode);

        addOperationCode(data);
        data.global.useUV = true;

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
bool BumpToNormal::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;

    size_t context_id = data.pushContext();
    bool inputValid = sampleInput(0, t, data);
    data.popContext();

    std::string contextName = qb::ctxtName(context_id);

    if(inputValid)
    {
        size_t op_id = data.getContext().getNextOperationId();
        size_t uvId = data.getContext().getUvId();

        std::string opcode = qb::varNew(op_id) + std::string("vec4(0.0,0.0,1.0,1.0)") + qb::opEnd();
        opcode += std::string("    float s") + std::to_string(op_id) + std::string(" = 1.0f/256.0f;\n");
        opcode += std::string("    ") + qb::varName(op_id) + std::string(".x = ") + qb::funCall(contextName, {qb::uvName(uvId) + std::string(" + vec2(-1,0)*s") + std::to_string(op_id)}) + std::string(".x") + qb::opEnd();
        opcode += std::string("    ") + qb::varName(op_id) + std::string(".x -= ") + qb::funCall(contextName, {qb::uvName(uvId) + std::string(" + vec2(1,0)*s") + std::to_string(op_id)}) + std::string(".x") + qb::opEnd();
        opcode += std::string("    ") + qb::varName(op_id) + std::string(".y = ") + qb::funCall(contextName, {qb::uvName(uvId) + std::string(" + vec2(0,-1)*s") + std::to_string(op_id)}) + std::string(".x") + qb::opEnd();
        opcode += std::string("    ") + qb::varName(op_id) + std::string(".y -= ") + qb::funCall(contextName, {qb::uvName(uvId) + std::string(" + vec2(0,1)*s") + std::to_string(op_id)}) + std::string(".x") + qb::opEnd();

        opcode += "vec3 va" + std::to_string(op_id) + " = normalize(vec3(2.0,0.0," + qb::varName(op_id) + ".x))" + qb::opEnd();
        opcode += "vec3 vb" + std::to_string(op_id) + " = normalize(vec3(0.0,2.0," + qb::varName(op_id) + ".y))" + qb::opEnd();
        opcode += qb::varName(op_id) + ".xyz = cross(va" + std::to_string(op_id) + ",vb" + std::to_string(op_id) + ")" + qb::opEnd();

        data.getContext().pushOperation(op_id, opcode);

        addOperationCode(data);
        data.global.useUV = true;

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
bool PerlinNoise::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    t.dstOp = this;
    
    size_t op_id = data.getContext().getNextOperationId();
    size_t input_id = data.global.pushUniform({(float)octaves, frequency, persistance, smoothness});
    size_t uvId = data.getContext().getUvId();
    std::string opcode = qb::varNew(op_id) + qb::funCall("perlin", {qb::inputName(input_id), {qb::uvName(uvId)}}) + qb::opEnd();

    data.getContext().pushOperation(op_id, opcode);

    addOperationCode(data);
    data.global.useUV = true;

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
bool HighResOutput::sample(size_t index, const Time& t, ImageOperationVisitor& data)
{
    size_t op_id;
    std::string opcode;

    if(sampleInput(0, t, data))
    {
        size_t var1 = data.getContext().popResultId();
        op_id = data.getContext().getNextOperationId();
        opcode = qb::varNew(op_id) + qb::varName(var1) + qb::opEnd();
    }
    else
    {
        size_t input_id = data.global.pushUniform({1.0f,1.0f,1.0f,1.0f});
        op_id = data.getContext().getNextOperationId();
        opcode = qb::varNew(op_id) + qb::inputName(input_id-1) + qb::opEnd();
    }
    data.getContext().pushOperation(op_id, opcode);

    addOperationCode(data);

    return true;
}