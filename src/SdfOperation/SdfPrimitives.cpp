#include "SdfOperation/SdfPrimitives.hpp"
#include "SdfOperation/RayMarcherBuilder.hpp"

#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"


//--------------------------------------------------------------
SdfPrimitive::SdfPrimitive(qb::SdfOperationType opType)
    : SdfOperation(opType)
{}

//--------------------------------------------------------------
void SdfPrimitive::prepareOperation(qb::RMBuilderVisitor& visitor, size_t& opId, size_t& tfmrId)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    opId = context.getNextVa();
    tfmrId = context.getTransformId();
}

//--------------------------------------------------------------
void SdfPrimitive::setupOperation(qb::RMBuilderVisitor& visitor, size_t opId, const std::string& glsl)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());
}

//--------------------------------------------------------------
Sphere::Sphere()
    : SdfPrimitive(qb::SdfOperationType_Sphere)
{
    makeProperty("radius", &r, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Sphere::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);
    
    size_t in1 = visitor.getCurrentFrame().pushInput({r, rgb[0], rgb[1], rgb[2]});
    std::string glsl = "vec4 $1 = vec4(sdSphere($2.xyz, $3.x),$3.yzw);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    setupOperation(visitor,opId,glsl);
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
    : SdfPrimitive(qb::SdfOperationType_Box)
{
    makeProperty("sx", &sx, 0.0f, 2.0f);
    makeProperty("sy", &sy, 0.0f, 2.0f);
    makeProperty("sz", &sz, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Box::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({sx, sy, sz, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdBox($2.xyz, $3.xyz),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
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
RoundBox::RoundBox()
    : SdfPrimitive(qb::SdfOperationType_RoundBox)
{
    makeProperty("sx", &sx, 0.0f, 2.0f);
    makeProperty("sy", &sy, 0.0f, 2.0f);
    makeProperty("sz", &sz, 0.0f, 2.0f);
    makeProperty("r", &r, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool RoundBox::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({sx, sy, sz, r});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdRoundBox($2.xyz, $3.xyz, $3.w),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
    return true;
}
//--------------------------------------------------------------
void RoundBox::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    for(auto i : {0,1,2,3}) uiProperty(i);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}

//--------------------------------------------------------------
Torus::Torus()
    : SdfPrimitive(qb::SdfOperationType_Torus)
{
    makeProperty("t1", &t1, 0.0f, 2.0f);
    makeProperty("t2", &t2, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Torus::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({t1, t2, 0.0, 0.0});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdTorus($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
    return true;
}
//--------------------------------------------------------------
void Torus::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    for(auto i : {0,1}) uiProperty(i);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}

//--------------------------------------------------------------
HexagonalPrism::HexagonalPrism()
    : SdfPrimitive(qb::SdfOperationType_HexPrism)
{
    makeProperty("h1", &h1, 0.0f, 2.0f);
    makeProperty("h2", &h2, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool HexagonalPrism::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({h1, h2, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdHexPrism($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
    return true;
}
//--------------------------------------------------------------
void HexagonalPrism::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    for(auto i : {0,1}) uiProperty(i);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}
//--------------------------------------------------------------
TriangularPrism::TriangularPrism()
    : SdfPrimitive(qb::SdfOperationType_TriPrism)
{
    makeProperty("t1", &t1, 0.0f, 2.0f);
    makeProperty("t2", &t2, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool TriangularPrism::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({t1, t2, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdTriPrism($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
    return true;
}
//--------------------------------------------------------------
void TriangularPrism::uiProperties()
{
    uiPreview();
    ImGui::BeginChild("##properties");
    for(auto i : {0,1}) uiProperty(i);
    if (ImGui::ColorEdit3("color", rgb))
        dirty(false);
    ImGui::EndChild();
}
//--------------------------------------------------------------
Capsule::Capsule()
    : SdfPrimitive(qb::SdfOperationType_Capsule)
{
    makeProperty("h", &h, 0.0f, 2.0f);
    makeProperty("r", &r, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Capsule::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({h, r, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdCapsule($2.xyz, $3.x, $3.y), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
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
    : SdfPrimitive(qb::SdfOperationType_Cone)
{
    makeProperty("r", &r, 0.0f, 2.0f);
    makeProperty("h", &h, 0.0f, 2.0f);
    makeProperty("red", &rgb[0], 0.0f, 1.0f);
    makeProperty("green", &rgb[1], 0.0f, 1.0f);
    makeProperty("blue", &rgb[2], 0.0f, 1.0f);
    makeOutput("out", BaseOperationDataType::Float);
}
//--------------------------------------------------------------
bool Cone::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().pushInput({r, h, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
    std::string glsl = "vec4 $1 = vec4(sdCone($2.xyz, $3.x, $3.y), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
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
    "    vec3 q = abs(p) - b;\n"
    "    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string RoundBox::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdRoundBox(vec3 p, vec3 b, float r){\n"
    "    vec3 q = abs(p) - b;\n"
    "    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - r;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string Torus::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdTorus(vec3 p, vec2 t){\n"
    "    vec2 q = vec2(length(p.xz)-t.x,p.y);\n"
    "    return length(q)-t.y;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string HexagonalPrism::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdHexPrism(vec3 p, vec2 h){\n"
    "    const vec3 k = vec3(-0.8660254, 0.5, 0.57735);\n"
    "    p = abs(p);\n"
    "    p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;\n"
    "    vec2 d = vec2(\n"
    "        length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),\n"
    "        p.z-h.y );\n"
    "    return min(max(d.x,d.y),0.0) + length(max(d,0.0));\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string TriangularPrism::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdTriPrism(vec3 p, vec2 h){\n"
    "    vec3 q = abs(p);\n"
    "    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);\n"
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