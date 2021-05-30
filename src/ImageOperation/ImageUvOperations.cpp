#include "ImageOperation/ImageUvOperations.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
UvInput::UvInput()
    : ImageOperation(qb::ImageOperationType_UvInput)
{
    makeOutput("uv", ImageDataType_Float);
}
//--------------------------------------------------------------
bool UvInput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
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
    makeInput("uv", ImageDataType_Float);
    makeInput("normals", ImageDataType_Float);
    makeOutput("uv", ImageDataType_Float);
    makeProperty("force", &_force, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool UvDistortion::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string inputUv, inputNormals;

    if(sampleInput(0, t, visitor)) inputUv = qb::va( context.popVa() );
    else inputUv = "uv0";
    if(sampleInput(1, t, visitor)) inputNormals = qb::va( context.popVa() );
    else inputNormals = "vec4(0.5,0.5,1.0,1.0)";

    std::string forceIn = qb::in( frame.pushInput({_force,_force,_force,_force}) );
    
    size_t opId = context.getNextVa();
    std::string glsl = "vec4 $1 = vec4($2.xy + ($3.xy - vec2(0.5)) * $4.x,0.0,1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), inputUv, inputNormals, forceIn});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());
    return true;
}
//--------------------------------------------------------------
UvMapping::UvMapping()
    : ImageOperation(qb::ImageOperationType_UvMapping)
{
    makeInput("in", ImageDataType_Float);
    makeInput("uv", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("u offset", &uOft, -1.0f, 1.0f);
    makeProperty("v offset", &vOft, -1.0f, 1.0f);
    makeProperty("u factor", &uFct, 0.0001f, 10.0f);
    makeProperty("v factor", &vFct, 0.0001f, 10.0f);
}
//--------------------------------------------------------------
bool UvMapping::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string glsl;

    bool inputValid = sampleInput(1, t, visitor); // uv map

    size_t uvId2 = context.getNextUv();
    size_t uvId = context.getUvId();

    if(inputValid)
    {
        size_t v1 = context.popVa();
        glsl = "vec2 $1 = uv_mapping($2,vec4($3.xy-uv0, 1.0,1.0));\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::va(v1)});
    }
    else
    {
        size_t in1 = frame.pushInput({uOft, vOft, uFct, vFct});
        glsl = "vec2 $1 = uv_mapping($2,$3);\n";
        glsl = qb::replaceArgs(glsl, {qb::uv(uvId2), qb::uv(uvId), qb::in(in1)});
    }
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    context.pushCode(glsl);
    context.pushUv(uvId2);
    bool ret = sampleInput(0, t, visitor);
    context.popUv();
    return ret;
}
//--------------------------------------------------------------
std::string UvMapping::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec2 uv_mapping(vec2 uv, vec4 oft){\n"
    "    return uv * oft.zw + oft.xy;\n"
    "}\n";
    return std::string(code);
}