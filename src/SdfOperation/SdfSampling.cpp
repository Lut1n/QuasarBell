#include "SdfOperation/SdfSampling.hpp"
#include "SdfOperation/RayMarcherBuilder.hpp"

#include "Core/Quat.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

namespace qb
{
    std::string getVec3ToUvCode()
    {
        static constexpr std::string_view code =
        "vec2 v3_to_uv(vec3 p)\n"
        "{\n"
        "    p = normalize(p);\n"
        "    \n"
        "    // layers\n"
        "    // 0: POS X   -Z -Y\n"
        "    // 1: NEG X   +Z -Y\n"
        "    // 2: POS Y   +X +Z\n"
        "    // 3: NEG Y   +X -Z\n"
        "    // 4: POS Z   +X -Y\n"
        "    // 5: NEG Z   -X -Y\n"
        "    vec3 u_map[6] = vec3[](\n"
        "        vec3(0,0,-1),\n"
        "        vec3(0,0,1),\n"
        "        vec3(1,0,0),\n"
        "        vec3(1,0,0),\n"
        "        vec3(1,0,0),\n"
        "        vec3(-1,0,0)\n"
        "    );\n"
        "    vec3 v_map[6] = vec3[](\n"
        "        vec3(0,-1,0),\n"
        "        vec3(0,-1,0),\n"
        "        vec3(0,0,1),\n"
        "        vec3(0,0,-1),\n"
        "        vec3(0,-1,0),\n"
        "        vec3(0,-1,0)\n"
        "    );\n"
        "    \n"
        "    vec2 ret = vec2(0.0,0.0);\n"
        "    \n"
        "    vec3 ap = abs(p);\n"
        "    vec3 sp = sign(p);\n"
        "    float d = max(max(ap.x,ap.y),ap.z);\n"
        "    for(int i=0; i<3; i++)\n"
        "    {\n"
        "        if (d == ap[i])\n"
        "        {\n"
        "            if (sp[i] >= 0.0)\n"
        "                ret = (vec2(dot(p,u_map[i*2]),dot(p,v_map[i*2])));\n"
        "            else\n"
        "                ret = (vec2(dot(p,u_map[i*2+1]),dot(p,v_map[i*2+1])));\n"
        "        }\n"
        "    }\n"
        "    return ret * 0.5 + 0.5;\n"
        "}\n";
        return std::string(code);
    }
};

//--------------------------------------------------------------
Displacement::Displacement()
    : SdfOperation(qb::SdfOperationType_Displacement)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float, UiPin::TYPE_FLOAT2);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("intensity",&intensity, 0.0, 1.0);
}

//--------------------------------------------------------------
bool Displacement::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t frameId = 0;
    if (sampleTextureInput(1, visitor, frameId))
    {
        std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
        std::string in2 = qb::in(visitor.getCurrentFrame().pushInput({intensity,0.0f,0.0f,0.0f}));
        std::string in3 = qb::sa(frameId);

        size_t opId = context.getNextVa();
        size_t tfmrId = context.getTransformId();

        std::string glsl = "vec4 $1 = opDisplacement($2, $3.xyz, $4.x, $5);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions("v3_to_uv", qb::getVec3ToUvCode());
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
std::string Displacement::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opDisplacement(vec4 v1, vec3 p, float intensity, sampler2D s2d){\n"
    "    vec2 s2d_uv = v3_to_uv(p);\n"
    "    return vec4(v1.x + intensity*(texture2D(s2d,s2d_uv).x * 2.0 - 1.0), v1.yzw);\n"
    "}\n";
    return std::string(code);
}


//--------------------------------------------------------------
Texturing::Texturing()
    : SdfOperation(qb::SdfOperationType_Texturing)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float, UiPin::TYPE_FLOAT2);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("factor",&factor, 0.0, 1.0);
}

//--------------------------------------------------------------
bool Texturing::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t frameId = 0;
    if (sampleTextureInput(1, visitor, frameId))
    {
        std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
        std::string in2 = qb::in(visitor.getCurrentFrame().pushInput({factor,0.0f,0.0f,0.0f}));
        std::string in3 = qb::sa(frameId);

        size_t opId = context.getNextVa();
        size_t tfmrId = context.getTransformId();

        std::string glsl = "vec4 $1 = opTexturing($2, $3.xyz, $4.x, $5);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions("v3_to_uv", qb::getVec3ToUvCode());
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
std::string Texturing::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opTexturing(vec4 v1, vec3 p, float factor, sampler2D s2d){\n"
    "    vec2 s2d_uv = v3_to_uv(p);\n"
    "    return vec4(v1.x, mix(v1.yzw,texture2D(s2d,s2d_uv).xyz,factor));\n"
    "}\n";
    return std::string(code);
}