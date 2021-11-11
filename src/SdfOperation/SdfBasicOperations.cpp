#include "SdfOperation/SdfBasicOperations.hpp"
#include "SdfOperation/RayMarcherBuilder.hpp"

#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
Union::Union()
    : SdfOperation(qb::SdfOperationType_Union)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Union::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opUnion($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
Substraction::Substraction()
    : SdfOperation(qb::SdfOperationType_Substraction)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}

//--------------------------------------------------------------
bool Substraction::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSubstraction($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
Intersection::Intersection()
    : SdfOperation(qb::SdfOperationType_Intersection)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
}

//--------------------------------------------------------------
bool Intersection::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opIntersection($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
SmoothUnion::SmoothUnion()
    : SdfOperation(qb::SdfOperationType_SmoothUnion)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("k",&k, 0.0, 3.0);
}

//--------------------------------------------------------------
bool SmoothUnion::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in3 = qb::in(visitor.getCurrentFrame().pushInput({k,k,k,k}));

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothUnion($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
SmoothSubstraction::SmoothSubstraction()
    : SdfOperation(qb::SdfOperationType_SmoothSubstraction)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("k",&k, 0.0, 3.0);
}

//--------------------------------------------------------------
bool SmoothSubstraction::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in3 = qb::in(visitor.getCurrentFrame().pushInput({k,k,k,k}));

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothSubstraction($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
SmoothIntersection::SmoothIntersection()
    : SdfOperation(qb::SdfOperationType_SmoothIntersection)
{
    makeInput("in1", BaseOperationDataType::Float);
    makeInput("in2", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("k",&k, 0.0, 3.0);
}

//--------------------------------------------------------------
bool SmoothIntersection::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = pushOpOrInput(0,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in2 = pushOpOrInput(1,visitor, {1e10f,1e10f,1e10f,1e10f});
    std::string in3 = qb::in(visitor.getCurrentFrame().pushInput({k,k,k,k}));

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothIntersection($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
std::string Union::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opUnion(vec4 d1, vec4 d2){\n"
    "    return d1.x<d2.x ? d1 : d2;\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Substraction::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opSubstraction(vec4 d1, vec4 d2){\n"
    "    return -d1.x>d2.x ? vec4(-d1.x,d1.yzw) : d2;\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Intersection::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opIntersection(vec4 d1, vec4 d2){\n"
    "    return d1.x>d2.x ? d1 : d2;\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string SmoothUnion::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opSmoothUnion(vec4 d1, vec4 d2, vec4 k){\n"
    "    float h = clamp(0.5 + 0.5 * (d2.x-d1.x)/k.x, 0.0, 1.0);\n"
    "    return vec4(mix(d2.x,d1.x,h) - k.x*h*(1.0-h),\n"
    "        mix(d2.yzw,d1.yzw,h) - k.x*h*(1.0-h));\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string SmoothSubstraction::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opSmoothSubstraction(vec4 d1, vec4 d2, vec4 k){\n"
    "    float h = clamp( 0.5 - 0.5*(d2.x+d1.x)/k.x, 0.0, 1.0 );\n"
    "    return vec4(mix( d2.x, -d1.x, h ) + k.x*h*(1.0-h),\n"
    "        mix( d2.yzw, -d1.yzw, h ) + k.x*h*(1.0-h));\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string SmoothIntersection::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opSmoothIntersection(vec4 d1, vec4 d2, vec4 k){\n"
    "    float h = clamp( 0.5 - 0.5*(d2.x-d1.x)/k.x, 0.0, 1.0 );\n"
    "    return vec4(mix( d2.x, d1.x, h ) + k.x*h*(1.0-h),\n"
    "        mix( d2.yzw, d1.yzw, h ) + k.x*h*(1.0-h));\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
HighResSdfOutput::HighResSdfOutput()
    : SdfOperation(qb::SdfOperationType_HighResOutput)
{
    preview.resolution = (int)std::pow(2.0,(float)resolution);
    makeInput("in", BaseOperationDataType::Float);
    makeProperty("resolution", BaseOperationDataType::Int, &resolution);
}
//--------------------------------------------------------------
HighResSdfOutput::~HighResSdfOutput()
{
}
//--------------------------------------------------------------
bool HighResSdfOutput::sample(size_t index, qb::GlslBuilderVisitor& visitor)
{
    size_t opId;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = pushOpOrInput(0,visitor, {1.0f,1.0f,1.0f,1.0f});

    opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void HighResSdfOutput::uiProperties()
{
    uiPreview();
    
    if (ImGui::SliderInt("power", &resolution, 0, 12))
    {
        preview.resolution = (int)std::pow(2.0,(float)resolution);
        preview.initialized = false;
        dirty(true);
    }
    int res = (int)std::pow(2.0,(float)resolution);
    ImGui::InputInt("resolution", &res);
}