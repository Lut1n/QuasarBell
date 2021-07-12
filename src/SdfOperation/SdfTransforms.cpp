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
    makeProperty("x",&x, -10.0, 10.0);
    makeProperty("y",&y, -10.0, 10.0);
    makeProperty("z",&z, -10.0, 10.0);
    makeProperty("rx",&rx, -180.0, 180.0);
    makeProperty("ry",&ry, -180.0, 180.0);
    makeProperty("rz",&rz, -180.0, 180.0);
}
//--------------------------------------------------------------
bool Transform::sample(size_t index, qb::RMBuilderVisitor& visitor)
{    
    const float degToRad = std::acos(-1.0) / 180.0f;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = qb::in(visitor.getCurrentFrame().pushInput({x,y,z,0.0f}));
    quat qx = quat(vec4(1.0,0.0,0.0,0.0),rx * degToRad);
    quat qy = quat(vec4(0.0,1.0,0.0,0.0),ry * degToRad);
    quat qz = quat(vec4(0.0,0.0,1.0,0.0),rz * degToRad);
    quat q = qx*(qy*qz);
    std::string in2 = qb::in(visitor.getCurrentFrame().pushInput(q));

    size_t pos0 = context.getTransformId();
    size_t opId = context.getNextTransform();
    size_t uvId = context.getUvId();

    std::string glsl = "vec4 $1 = opTransform($2.xyz, $3.xyz, $4);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(opId), qb::tfmr(pos0), in1, in2});

    context.pushCode(glsl);
    frame.hasUv = true;

    frame.setFunctions(getNodeType(), getOperationCode());

    context.pushTransform(opId);    
    bool ret = sampleInput(0, visitor);
    context.popTransform();

    return ret;
}

//--------------------------------------------------------------
std::string Transform::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 multQuat(vec4 q1, vec4 q2){\n"
    "    return vec4(\n"
    "        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,\n"
    "        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,\n"
    "        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,\n"
    "        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z\n"
    "    );\n"
    "}\n"
    "vec4 opTransform(vec3 pos, vec3 t, vec4 q){\n"
    "    vec4 p = vec4(pos-t,0.0);\n"
    "    vec4 qi = vec4(-q.x,-q.y,-q.z,q.w);\n"
    "    p = multQuat(q,multQuat(p,qi));\n"
    "    return vec4(p.xyz,0.0);\n"
    "}\n";
    return std::string(code);
}