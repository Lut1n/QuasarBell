#include "SdfOperation/SdfPrimitives.hpp"
#include "SdfOperation/RayMarcherBuilder.hpp"

#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
Sphere::Sphere()
    : SdfOperation(qb::SdfOperationType_Sphere)
{
    makeProperty("radius", &r, 0.0f, 10.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Sphere::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    size_t in1 = frame.pushInput({r, r, r, r});
    std::string glsl = "vec4 $1 = sdSphere($2.xyz, $3);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
Box::Box()
    : SdfOperation(qb::SdfOperationType_Box)
{
    makeProperty("sx", &sx, 0.0f, 10.0f);
    makeProperty("sy", &sy, 0.0f, 10.0f);
    makeProperty("sz", &sz, 0.0f, 10.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Box::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    size_t in1 = frame.pushInput({sx, sy, sz, 0.0f});
    std::string glsl = "vec4 $1 = sdBox($2.xyz, $3);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}

//--------------------------------------------------------------
Capsule::Capsule()
    : SdfOperation(qb::SdfOperationType_Capsule)
{
    makeProperty("h", &h, 0.0f, 10.0f);
    makeProperty("r", &r, 0.0f, 10.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Capsule::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    size_t in1 = frame.pushInput({h, r, 0.0f, 0.0f});
    std::string glsl = "vec4 $1 = sdCapsule($2.xyz, $3);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
std::string Sphere::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 sdSphere(vec3 p, vec4 s){\n"
    "      return vec4(length(p)-s.w);\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Box::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 sdBox(vec3 p, vec4 b){\n"
    "    vec3 q = abs(p) - b.xyz;\n"
    "    return vec4(length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0));\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Capsule::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 sdCapsule(vec3 p, vec4 hr){\n"
    "    p.y -= clamp(p.y, 0.0, hr.x);\n"
    "    return vec4(length(p) - hr.y);\n"
    "}\n";
    return std::string(code);
}