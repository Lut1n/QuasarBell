#include "SdfOperation/SdfTransforms.hpp"
#include "SdfOperation/RayMarcherBuilder.hpp"

#include "Core/Quat.hpp"
#include "ImageOperation/GlslBuilder.hpp"

#include <array>
#include <string>
#include "imgui.h"

#include "Ui/UiSystem.h"

//--------------------------------------------------------------
Transform::Transform()
    : SdfOperation(qb::SdfOperationType_Transform)
{
    makeInput("in", BaseOperationDataType::Float);
    makeOutput("out", BaseOperationDataType::Float);
    makeProperty("x",&x, -2.0, 2.0);
    makeProperty("y",&y, -2.0, 2.0);
    makeProperty("z",&z, -2.0, 2.0);
    makeProperty("rx",&rx, -180.0, 180.0);
    makeProperty("ry",&ry, -180.0, 180.0);
    makeProperty("rz",&rz, -180.0, 180.0);
    makeProperty("s",&s, 0.1, 2.0);
}
//--------------------------------------------------------------
void Transform::startSampling(int d)
{
    showGizmo = (d==0);
}
//--------------------------------------------------------------
bool Transform::sample(size_t index, qb::RMBuilderVisitor& visitor)
{
    const float degToRad = std::acos(-1.0) / 180.0f;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = qb::in(visitor.getCurrentFrame().pushInput({x,y,z,s}));
    quat qx = quat(vec4(1.0,0.0,0.0,0.0),rx * degToRad);
    quat qy = quat(vec4(0.0,1.0,0.0,0.0),ry * degToRad);
    quat qz = quat(vec4(0.0,0.0,1.0,0.0),rz * degToRad);
    quat q = qx*(qy*qz);
    std::string in2 = qb::in(visitor.getCurrentFrame().pushInput(q));

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = opTransform($2.xyz, $3.xyz, $4, $3.w);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in1, in2});

    context.pushCode(glsl);

    frame.setFunctions(getNodeType(), getOperationCode());

    context.pushTransform(pos1);
    bool ret = sampleInput(0, visitor);
    if(showGizmo && ret)
    {
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::string op0 = qb::va(context.popVa());

        size_t op1 = context.getNextVa();

        std::string glsl =
        "vec4 gizmo_pos_x = opTransform($3.xyz, vec3(0.0), vec4(0.0,0.0,0.7,0.7), 1.0);\n"
        "vec4 gizmo_pos_y = vec4($3.xyz,0.0);\n"
        "vec4 gizmo_pos_z = opTransform($3.xyz, vec3(0.0), vec4(-0.7,0.0,0.0,0.7), 1.0);\n"
        "vec4 gizmo_x = vec4(sdGizmo(gizmo_pos_x.xyz),1.0,0.0,0.0);\n"
        "vec4 gizmo_y = vec4(sdGizmo(gizmo_pos_y.xyz),0.0,1.0,0.0);\n"
        "vec4 gizmo_z = vec4(sdGizmo(gizmo_pos_z.xyz),0.0,0.0,1.0);\n"
        "vec4 $1 = $2.x<gizmo_x.x?$2:gizmo_x;\n"
        "$1 = $1.x<gizmo_y.x?$1:gizmo_y;\n"
        "$1 = $1.x<gizmo_z.x?$1:gizmo_z;\n"
        "$1.x = $1.x*$4.w;\n";
        glsl = qb::replaceArgs(glsl, {qb::va(op1), op0, qb::tfmr(pos1), in1});

        context.pushVa(op1);
        context.pushCode(glsl);

        return true;
    }
    context.popTransform();

    if (ret)
    {
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::string op2 = qb::va(context.popVa());
        size_t op3 = context.getNextVa();

        std::string glslPop = "vec4 $1 = opTransformPop($2, $3.w);\n";
        glslPop = qb::replaceArgs(glslPop, {qb::va(op3), op2, in1});
        
        context.pushVa(op3);
        context.pushCode(glslPop);
    }

    return ret;
}

//--------------------------------------------------------------
std::string Transform::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdGizmo(vec3 p){\n"
    "    vec2 q = vec2(0.1,0.5);\n"
    "    vec2 w = vec2(length(p.xz), p.y);\n"
    "    vec2 a = w - q*clamp(dot(w,q)/dot(q,q), 0.0,1.0);\n"
    "    vec2 b = w - q*vec2(clamp( w.x/q.x, 0.0, 1.0),1.0);\n"
    "    float k = sign(q.y);\n"
    "    float d = min(dot(a,a),dot(b, b));\n"
    "    float s = max(k*(w.x*q.y-w.y*q.x),k*(w.y-q.y));\n"
    "    return sqrt(d)*sign(s);\n"
    "}\n"
    "vec4 multQuat(vec4 q1, vec4 q2){\n"
    "    return vec4(\n"
    "        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,\n"
    "        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,\n"
    "        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,\n"
    "        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z\n"
    "    );\n"
    "}\n"
    "vec4 opTransform(vec3 pos, vec3 t, vec4 q, float s){\n"
    "    vec4 p = vec4(pos/s-t,0.0);\n"
    "    vec4 qi = vec4(-q.x,-q.y,-q.z,q.w);\n"
    "    p = multQuat(q,multQuat(p,qi));\n"
    "    return vec4(p.xyz,0.0);\n"
    "}\n"
    "vec4 opTransformPop(vec4 v, float s){\n"
    "    return vec4(v.x*s,v.yzw);\n"
    "}\n";
    return std::string(code);
}