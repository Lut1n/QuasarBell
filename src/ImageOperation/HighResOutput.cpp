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
bool ColorInput::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;

    size_t input_id = data.collectedUniforms.size();
    size_t op_id = data.collectedOperations.size();

    ImageOperationData::vec4 color = {r,g,b,a};
    std::string opcode = qb::varNew(op_id) + qb::inputName(input_id) + qb::opEnd();

    data.collectedUniforms.push_back(color);
    data.collectedOperations.push_back(opcode);

    addOperationCode(data);
    data.unusedVars.push_back(op_id);

    return true;
}
//--------------------------------------------------------------
void ColorInput::uiProperties()
{
    uiPreview();
    ImageOperationData::vec4 color = {r,g,b,a};
    if (ImGui::ColorEdit4("color", (float*)&color))
    {
        r = color.x; g = color.y; b = color.z; a = color.w;
        dirty();
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
bool ImageMult::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;
    
    size_t var1, var2;
    if(!sampleInput(0, t, data)) return false;
    var1 = data.unusedVars.front(); data.unusedVars.pop_front();
    if(!sampleInput(1, t, data)) return false;
    var2 = data.unusedVars.front(); data.unusedVars.pop_front();

    size_t op_id = data.collectedOperations.size();
    std::string opcode = qb::varNew(op_id) + qb::varName(var1) + std::string(" * ") + qb::varName(var2) + qb::opEnd();
    data.collectedOperations.push_back(opcode);
    data.unusedVars.push_back(op_id);
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
bool ImageMix::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;

    size_t var1, var2;
    if(!sampleInput(0, t, data)) return false;
    var1 = data.unusedVars.front(); data.unusedVars.pop_front();
    if(!sampleInput(1, t, data)) return false;
    var2 = data.unusedVars.front(); data.unusedVars.pop_front();

    std::vector<std::string> args;
    if (sampleInput(2, t, data))
    {
        size_t var3 = data.unusedVars.front(); data.unusedVars.pop_front();
        args = {qb::varName(var1), qb::varName(var2), qb::varName(var3)};
    }
    else
    {
        std::string vec4str = std::string("vec4(") + std::to_string(delta) + std::string(")");
        args = {qb::varName(var1), qb::varName(var2), vec4str};
    }
    
    size_t op_id = data.collectedOperations.size();
    std::string opcode = qb::varNew(op_id) + qb::funCall("mix", args) + qb::opEnd();
    data.collectedOperations.push_back(opcode);
    data.unusedVars.push_back(op_id);
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
bool Dynamics::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;
    
    if(sampleInput(0, t, data))
    {
        size_t var1 = data.unusedVars.front(); data.unusedVars.pop_front();
        size_t input_id = data.collectedUniforms.size();
        size_t op_id = data.collectedOperations.size();
        ImageOperationData::vec4 bc = {brightness,contrast,0.0f,0.0f};
        data.collectedUniforms.push_back(bc);
        std::string opcode = qb::varNew(op_id) + qb::funCall("dynamics", {qb::varName(var1), qb::inputName(input_id)}) + qb::opEnd();
        data.collectedOperations.push_back(opcode);
        data.unusedVars.push_back(op_id);
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
bool WhiteNoise::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;

    size_t op_id = data.collectedOperations.size();
    std::string opcode = qb::varNew(op_id) + qb::funCall("white_noise", {}) + qb::opEnd();

    data.collectedOperations.push_back(opcode);
    data.unusedVars.push_back(op_id);
    data.useUV = true;

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
    "vec4 white_noise(){\n"
    "    float r = rand_wnoise(vUV);\n"
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
bool PerlinNoise::sample(size_t index, const Time& t, ImageOperationData& data)
{
    t.dstOp = this;
    std::string opcode;
    
    size_t input_id = data.collectedUniforms.size();
    size_t op_id = data.collectedOperations.size();
    ImageOperationData::vec4 color = {(float)octaves, frequency, persistance, smoothness};
    data.collectedUniforms.push_back(color);
    opcode = qb::varNew(op_id) + qb::funCall("perlin", {qb::inputName(input_id)}) + qb::opEnd();

    data.collectedOperations.push_back(opcode);
    data.unusedVars.push_back(op_id);
    data.useUV = true;

    addOperationCode(data);

    return true;
}

//--------------------------------------------------------------
std::string PerlinNoise::getOperationCode() const
{
    static constexpr std::string_view code =
    "float rand_perlin(vec2 uv){\n"
    "    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "vec4 perlin(vec4 res){\n"
    "    vec2 uv = vUV;\n"
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
bool HighResOutput::sample(size_t index, const Time& t, ImageOperationData& data)
{
    std::string opcode;
    size_t op_id;
    if(sampleInput(0, t, data))
    {
        size_t var1 = data.unusedVars.front(); data.unusedVars.pop_front();
        op_id = data.collectedOperations.size();
        opcode = qb::varNew(op_id) + qb::varName(var1) + qb::opEnd();
    }
    else
    {
        size_t input_id = data.collectedUniforms.size();
        op_id = data.collectedOperations.size();
        ImageOperationData::vec4 color = {1.0f,1.0f,1.0f,1.0f};
        data.collectedUniforms.push_back(color);
        opcode = qb::varNew(op_id) + qb::inputName(input_id-1) + qb::opEnd();
    }
    data.collectedOperations.push_back(opcode);
    data.unusedVars.push_back(op_id);

    addOperationCode(data);

    return true;
}