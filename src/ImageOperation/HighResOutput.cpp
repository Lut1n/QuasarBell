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
}
//--------------------------------------------------------------
bool ColorInput::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
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
DirectionalSignal::DirectionalSignal()
    : ImageOperation(qb::ImageOperationType_DirSignal)
{
    makeProperty("direction x", &directionX, -1.0, 1.0);
    makeProperty("direction y", &directionY, -1.0, 1.0);
    makeProperty("frequency", &frequency, 1.0, 10.0);
    makeProperty("amplitude", &amplitude, 0.0, 1.0);
    makeOutput("out", ImageDataType_Float);
}
//--------------------------------------------------------------
bool DirectionalSignal::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({directionX,directionY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($3.w * (sin( dot($2.xy,normalize($3.xy)) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}
//--------------------------------------------------------------
RadialSignal::RadialSignal()
    : ImageOperation(qb::ImageOperationType_RadSignal)
{
    makeProperty("center x", &centerX, 0.0, 1.0);
    makeProperty("center y", &centerY, 0.0, 1.0);
    makeProperty("frequency", &frequency, 1.0, 10.0);
    makeProperty("amplitude", &amplitude, 0.0, 1.0);
    makeOutput("output", ImageDataType_Float);
}
//--------------------------------------------------------------
bool RadialSignal::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t inId = frame.pushInput({centerX,centerY,frequency,amplitude});
    size_t uvId = context.getUvId();
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(vec3($3.w * (sin( distance($2.xy,$3.xy) * $3.z * 6.28 ) * 0.5 + 0.5)),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::uv(uvId), qb::in(inId)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.setFunctions(getNodeType(), getOperationCode());
    frame.hasUv = true;

    return true;
}
//--------------------------------------------------------------
ImageMix::ImageMix()
    : ImageOperation(qb::ImageOperationType_Mix)
{
    makeInput("in1", ImageDataType_Float);
    makeInput("in2", ImageDataType_Float);
    makeInput("delta", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("delta", &delta, 0.0f, 1.0f);
}
//--------------------------------------------------------------
bool ImageMix::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();
    if(!sampleInput(1, t, visitor)) return false;
    size_t v2 = context.popVa();

    bool inputValid = sampleInput(2, t, visitor);
    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(mix($2.xyz, $3.xyz, $4.xyz),1.0);\n";
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
ImageClamp::ImageClamp()
    : ImageOperation(qb::ImageOperationType_Clamp)
{
    makeInput("in", ImageDataType_Float);
    makeInput("edge0", ImageDataType_Float);
    makeInput("edge1", ImageDataType_Float);
    makeOutput("out", ImageDataType_Float);
    makeProperty("edge0", ImageDataType_Float, &edge0);
    makeProperty("edge1", ImageDataType_Float, &edge1);
}
//--------------------------------------------------------------
bool ImageClamp::sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    if(!sampleInput(0, t, visitor)) return false;
    size_t v1 = context.popVa();

    std::string edge1Id, edge2Id;

    if (sampleInput(1, t, visitor))
        edge1Id = qb::va(context.popVa());
    else
        edge1Id = qb::in(frame.pushInput({edge0,edge0,edge0,edge0}));

    if (sampleInput(2, t, visitor))
        edge2Id = qb::va(context.popVa());
    else
        edge2Id = qb::in(frame.pushInput({edge1,edge1,edge1,edge1}));

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = vec4(clamp($2.xyz, $3.xyz, $4.xyz),1.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::va(v1), edge1Id, edge2Id});
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