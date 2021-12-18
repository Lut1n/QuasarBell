#include "SdfOperation/SdfSampling.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include "Core/Quat.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

namespace qb
{
    std::string getSdfFetcherCode()
    {
        static constexpr std::string_view code =
        
        "vec2 toPolar(vec3 p)\n"
        "{\n"
        "    #define PI 3.1415926535897932384626433832795\n"
        "    p = normalize(p);\n"
        "    float a = (atan(p.z,p.x) + PI) * 0.5;\n"
        "    float b = acos(p.y);\n"
        "    return vec2(a,b) / PI;\n"
        "}\n"
        "vec4 fetchSphereMap(sampler2D s2d, vec3 p)\n"
        "{\n"
        "    vec2 s2d_uv = toPolar(p);\n"
        "    return texture2D(s2d,s2d_uv);\n"
        "}\n"
        "vec4 fetchTriplanarMap(sampler2D s2d, vec3 p)\n"
        "{\n"
        "    p = normalize(p);\n"
        "    vec4 x = texture2D(s2d, p.yz);\n"
        "    vec4 y = texture2D(s2d, p.zx);\n"
        "    vec4 z = texture2D(s2d, p.xy);\n"
        "    vec3 m = pow(abs(p), vec3(2.2));\n"
        "    return (x*m.x+y*m.y+z*m.z) / (m.x+m.y+m.z);\n"
        "}\n"
        "vec4 fetchCubeMap(sampler2D s2d, vec3 p)\n"
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
        "    vec4 ret = vec4(0.0,0.0,0.0,0.0);\n"
        "    vec3 ap = abs(p);\n"
        "    vec3 sp = sign(p);\n"
        "    float d = max(max(ap.x,ap.y),ap.z);\n"
        "    for(int i=0; i<3; i++)\n"
        "    {\n"
        "        if (d == ap[i])\n"
        "        {\n"
        "            vec2 uv = vec2(0.0,0.0);\n"
        "            if (sp[i] >= 0.0)\n"
        "                uv = vec2( dot(p,u_map[i*2]), dot(p,v_map[i*2]) );\n"
        "            else\n"
        "                uv = vec2( dot(p,u_map[i*2+1]), dot(p,v_map[i*2+1]) );\n"
        "            ret = texture2D(s2d,uv * 0.5 + 0.5);\n"
        "        }\n"
        "    }\n"
        "    return ret;\n"
        "}\n";
        return std::string(code);
    }
};

//--------------------------------------------------------------
Displacement::Displacement()
    : SdfOperation(qb::SdfOperationType_Displacement)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float, UiPin::Type_S2d);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("intensity",&intensity, 0.0, 1.0);
    makeProperty("mode",&mode, 0, 2);
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
        std::string in2 = qb::in(visitor.getCurrentFrame().pushInput({intensity,(float)mode,0.0f,0.0f}));
        std::string in3 = qb::sa(frameId);

        size_t opId = context.getNextVa();
        size_t tfmrId = context.getTransformId();

        std::string glsl = "vec4 $1 = opDisplacement($2, $3.xyz, $4.xy, $5);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions("sdfFetcher", qb::getSdfFetcherCode());
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
std::string Displacement::getOperationCode() const
{
    static constexpr std::string_view code =
     "vec4 opDisplacement(vec4 v1, vec3 p, vec2 intensityMode, sampler2D s2d){\n"
     "    int mode = int(intensityMode.y);\n"
     "    vec4 ret = vec4(0.0);\n"
     "    if (mode == 0)\n"
     "      ret = fetchCubeMap(s2d, p);\n"
     "    else if (mode == 1)\n"
     "      ret = fetchSphereMap(s2d, p);\n"
     "    else if (mode == 2)\n"
     "      ret = fetchTriplanarMap(s2d, p);\n"
     "    return vec4(v1.x + intensityMode.x*(ret.x * 2.0 - 1.0),v1.yzw);\n"
     "}\n";

    return std::string(code);
}


//--------------------------------------------------------------
Texturing::Texturing()
    : SdfOperation(qb::SdfOperationType_Texturing)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float, UiPin::Type_S2d);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("factor",&factor, 0.0, 1.0);
    makeProperty("mode",&mode, 0, 2);
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
        std::string in2 = qb::in(visitor.getCurrentFrame().pushInput({factor,(float)mode,0.0f,0.0f}));
        std::string in3 = qb::sa(frameId);

        size_t opId = context.getNextVa();
        size_t tfmrId = context.getTransformId();

        std::string glsl = "vec4 $1 = opTexturing($2, $3.xyz, $4.xy, $5);\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

        context.pushVa(opId);
        context.pushCode(glsl);

        frame.setFunctions("sdfFetcher", qb::getSdfFetcherCode());
        frame.setFunctions(getNodeType(), getOperationCode());
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
std::string Texturing::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opTexturing(vec4 v1, vec3 p, vec2 factorMode, sampler2D s2d){\n"
     "    int mode = int(factorMode.y);\n"
     "    vec4 ret = vec4(0.0);\n"
     "    if (mode == 0)\n"
     "      ret = fetchCubeMap(s2d, p);\n"
     "    else if (mode == 1)\n"
     "      ret = fetchSphereMap(s2d, p);\n"
     "    else if (mode == 2)\n"
     "      ret = fetchTriplanarMap(s2d, p);\n"
     "    return vec4(v1.x, mix(v1.yzw,ret.xyz,factorMode.x));\n"
    "}\n";
    return std::string(code);
}