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
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Sphere::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    size_t in1 = frame.pushInput({r, rgb[0], rgb[1], rgb[2]});
    std::string glsl = "vec4 $1 = vec4(sdSphere($2.xyz, $3.x),$3.yzw);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void Sphere::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    uiProperty(0);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}

//--------------------------------------------------------------
Box::Box()
    : SdfOperation(qb::SdfOperationType_Box)
{
    makeProperty("sx", &sx, 0.0f, 10.0f);
    makeProperty("sy", &sy, 0.0f, 10.0f);
    makeProperty("sz", &sz, 0.0f, 10.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
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
    size_t in2 = frame.pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdBox($2.xyz, $3.xyz),$4.xyz);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void Box::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    uiProperty(0);
    uiProperty(1);
    uiProperty(2);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}

//--------------------------------------------------------------
Capsule::Capsule()
    : SdfOperation(qb::SdfOperationType_Capsule)
{
    makeProperty("h", &h, 0.0f, 10.0f);
    makeProperty("r", &r, 0.0f, 10.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
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
    size_t in2 = frame.pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdCapsule($2.xyz, $3.x, $3.y), $4.xyz);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void Capsule::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    uiProperty(0);
    uiProperty(1);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}

//--------------------------------------------------------------
Cone::Cone()
    : SdfOperation(qb::SdfOperationType_Cone)
{
    makeProperty("r", &r, 0.0f, 10.0f);
    makeProperty("h", &h, 0.0f, 10.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Cone::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t opId = context.getNextVa();
    size_t uvId = context.getUvId();
    size_t in1 = frame.pushInput({r, h, 0.0f, 0.0f});
    size_t in2 = frame.pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdCone($2.xyz, $3.x, $3.y), $4.xyz);\n";

    size_t tfmrId = context.getTransformId();
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    context.pushVa(opId);
    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    return true;
}
//--------------------------------------------------------------
void Cone::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    uiProperty(0);
    uiProperty(1);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}
//--------------------------------------------------------------
std::string Sphere::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdSphere(vec3 p, float s){\n"
    "      return length(p)-s;\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Box::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdBox(vec3 p, vec3 b){\n"
    "    vec3 q = abs(p) - b.xyz;\n"
    "    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Capsule::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdCapsule(vec3 p, float h, float r){\n"
    "    p.y -= clamp(p.y, 0.0, h);\n"
    "    return length(p) - r;\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
std::string Cone::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdCone(vec3 p, float r, float h){\n"
    "    vec2 q = vec2(r,h);\n"
    "    vec2 w = vec2(length(p.xz), p.y);\n"
    "    vec2 a = w - q*clamp(dot(w,q)/dot(q,q), 0.0,1.0);\n"
    "    vec2 b = w - q*vec2(clamp( w.x/q.x, 0.0, 1.0),1.0);\n"
    "    float k = sign(q.y);\n"
    "    float d = min(dot(a,a),dot(b, b));\n"
    "    float s = max(k*(w.x*q.y-w.y*q.x),k*(w.y-q.y));\n"
    "    return sqrt(d)*sign(s);\n"
    "}\n";
    return std::string(code);
}