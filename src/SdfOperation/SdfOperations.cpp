#include "SdfOperation/SdfOperations.hpp"

#include "Core/Quat.hpp"
#include "SdfOperation/VoxelExporter.hpp"

#include <algorithm>
#include <string>

//--------------------------------------------------------------
void prepareOperation(qb::GlslBuilderVisitor& visitor, size_t& opId, size_t& tfmrId)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    opId = context.getNextVa();
    tfmrId = context.getTransformId();
}
//--------------------------------------------------------------
void setupOperation(qb::GlslBuilderVisitor& visitor, size_t opId, const std::string& glsl)
{
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void Sphere::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdSphere($2.xyz, $3.x),$3.yzw);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SphereData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Box::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& sx = attributes->sx;
    auto& sy = attributes->sy;
    auto& sz = attributes->sz;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdBox($2.xyz, $3.xyz),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)BoxData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void RoundBox::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& sx = attributes->sx;
    auto& sy = attributes->sy;
    auto& sz = attributes->sz;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdRoundBox($2.xyz, $3.xyz, $3.w),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)RoundBoxData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Torus::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& t1 = attributes->t1;
    auto& t2 = attributes->t2;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdTorus($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)TorusData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void HexagonalPrism::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h1 = attributes->h1;
    auto& h2 = attributes->h2;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdHexPrism($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)HexagonalPrismData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void TriangularPrism::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& t1 = attributes->t1;
    auto& t2 = attributes->t2;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdTriPrism($2.xyz, $3.xy),$4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)TriangularPrismData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Capsule::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdCapsule($2.xyz, $3.x, $3.y), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)CapsuleData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Cone::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdCone($2.xyz, $3.x, $3.y), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)ConeData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void RoundedCone::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r1 = attributes->r1;
    auto& r2 = attributes->r2;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdRoundedCone($2.xyz, $3.x, $3.y, $3.z), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)RoundedConeData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Cylinder::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdCylinder($2.xyz, $3.x, $3.y), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)CylinderData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void RoundedCylinder::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r1 = attributes->r1;
    auto& r2 = attributes->r2;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdRoundedCylinder($2.xyz, $3.x, $3.y, $3.z), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)RoundedCylinderData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Octahedron::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& s = attributes->s;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdOctahedron($2.xyz, $3.x), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)OctahedronData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Pyramid::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;

    auto& visitor = result.visitor;
    size_t opId, tfmrId;
    prepareOperation(visitor,opId,tfmrId);

    size_t in1 = visitor.getCurrentFrame().uniformPlaceholder();
    size_t in2 = visitor.getCurrentFrame().uniformPlaceholder();
    std::string glsl = "vec4 $1 = vec4(sdPyramid($2.xyz, $3.x), $4.xyz);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), qb::tfmr(tfmrId), qb::in(in1), qb::in(in2)});

    setupOperation(visitor,opId,glsl);
	visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)PyramidData::TypeId, getOperationCode());
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
//--------------------------------------------------------------
std::string RoundedCone::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdRoundedCone(vec3 p, float r1, float r2, float h){\n"
    "    vec2 q = vec2(length(p.xz), p.y);\n"
    "    float b = (r1-r2)/h;\n"
    "    float a = sqrt(1.0-b*b);\n"
    "    float k = dot(q, vec2(-b,a));\n"
    "    if (k<0.0) return length(q) - r1;\n"
    "    if (k>a*h) return length(q-vec2(0.0,h)) - r2;\n"
    "    return dot(q,vec2(a,b)) - r1;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string Cylinder::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdCylinder(vec3 p, float h, float r){\n"
    "    vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(h,r);\n"
    "    return min(max(d.x,d.y),0.0) + length(max(d,0.0));\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string RoundedCylinder::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdRoundedCylinder(vec3 p, float r1, float r2, float h){\n"
    "    vec2 d = vec2( length(p.xz)-2.0*r1+r2, abs(p.y) - h );\n"
    "    return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - r2;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string Octahedron::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdOctahedron(vec3 p, float s){\n"
    "    p = abs(p);\n"
    "    float m = p.x+p.y+p.z-s;\n"
    "    vec3 q;\n"
    "    if( 3.0*p.x < m ) q = p.xyz;\n"
    "    else if( 3.0*p.y < m ) q = p.yzx;\n"
    "    else if( 3.0*p.z < m ) q = p.zxy;\n"
    "    else return m*0.57735027;\n"
    "    float k = clamp(0.5*(q.z-q.y+s),0.0,s); \n"
    "    return length(vec3(q.x,q.y-s+k,q.z-k)); \n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
std::string Pyramid::getOperationCode() const
{
    static constexpr std::string_view code =
    "float sdPyramid(vec3 p, float h){\n"
    "    float m2 = h*h + 0.25;\n"
    "    p.xz = abs(p.xz);\n"
    "    p.xz = (p.z>p.x) ? p.zx : p.xz;\n"
    "    p.xz -= 0.5;\n"
    "    vec3 q = vec3( p.z, h*p.y - 0.5*p.x, h*p.x + 0.5*p.y);\n"
    "    float s = max(-q.x,0.0);\n"
    "    float t = clamp( (q.y-0.5*p.z)/(m2+0.25), 0.0, 1.0 );\n"
    "    float a = m2*(q.x+s)*(q.x+s) + q.y*q.y;\n"
    "    float b = m2*(q.x+0.5*t)*(q.x+0.5*t) + (q.y-m2*t)*(q.y-m2*t);\n"
    "    float d2 = min(q.y,-q.x*m2-q.y*0.5) > 0.0 ? 0.0 : min(a,b);\n"
    "    return sqrt( (d2+q.z*q.z)/m2 ) * sign(max(q.z,-p.y));\n"
    "}\n";
    return std::string(code);
}

//--------------------------------------------------------------
void Union::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opUnion($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)UnionData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Substraction::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSubstraction($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SubstractionData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void Intersection::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opIntersection($2,$3);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)IntersectionData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void SmoothUnion::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);
    std::string in3 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothUnion($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SmoothUnionData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void SmoothSubstraction::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);
    std::string in3 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothSubstraction($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SmoothSubstractionData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
void SmoothIntersection::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = visitor.inputOrUniformPlaceholder(result, inputs[1], true);
    std::string in3 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());

    size_t opId = context.getNextVa();

    std::string glsl = "vec4 $1 = opSmoothIntersection($2,$3,$4);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, in2, in3});

    context.pushVa(opId);
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SmoothIntersectionData::TypeId, getOperationCode());
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
void HighResSdfOutput::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    size_t opId;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string valueId = visitor.inputOrUniformPlaceholder(result,inputs[0], true);

    opId = context.getNextVa();

    std::string glsl = "vec4 $1 = $2;\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});

    context.pushVa(opId);
    context.pushCode(glsl);
}
//--------------------------------------------------------------
void Transform::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;
    auto& rx = attributes->rx;
    auto& ry = attributes->ry;
    auto& rz = attributes->rz;
    auto& s = attributes->s;

    auto& visitor = result.visitor;
    const float degToRad = std::acos(-1.0) / 180.0f;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string posArg;
    std::string rotArg;
    std::string scaArg;

    size_t pFrameId = 0;
    size_t rFrameId = 0;
    size_t sFrameId = 0;

    bool sampleP = visitor.sampleInFrame(result, inputs[1], pFrameId);
    bool sampleR = visitor.sampleInFrame(result, inputs[2], rFrameId);
    bool sampleS = visitor.sampleInFrame(result, inputs[3], sFrameId);

    if (sampleP)
        posArg = "texture2D(" + qb::sa(pFrameId) + ",vec2(0.0,0.0)).xyz * 4.0 - 2.0";
    if (sampleS)
        scaArg = "mix(0.1,2.0,texture2D(" + qb::sa(sFrameId) + ",vec2(0.0,0.0)).x)";
    if (!sampleP || !sampleS)
    {
        std::string inPosSca = qb::in(visitor.getCurrentFrame().uniformPlaceholder());
        if (!sampleP)
            posArg = inPosSca + ".xyz";
        if (!sampleS)
            scaArg = inPosSca + ".w";
    }
    if (sampleR)
    {
        rotArg = "mkQuatFromRot(texture2D(" + qb::sa(rFrameId) + ",vec2(0.0,0.0)).xyz * 2.0*3.141592 - 3.141592)";
    }
    else
    {
        quat qx = quat(vec4(1.0,0.0,0.0,0.0),rx * degToRad);
        quat qy = quat(vec4(0.0,1.0,0.0,0.0),ry * degToRad);
        quat qz = quat(vec4(0.0,0.0,1.0,0.0),rz * degToRad);
        quat q = qx*(qy*qz);
        rotArg = qb::in(visitor.getCurrentFrame().uniformPlaceholder());
    }

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = opTransform($2.xyz, $3, $4, $5);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), posArg, rotArg, scaArg});

    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)TransformData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    bool inputValid = visitor.sampleInput(result, inputs[0]);
    bool showGizmo = result.lastAttributes == attributes;
    if(showGizmo)
    {
        auto& frame = visitor.getCurrentFrame();
        auto& context = frame.getContext();

        std::string op0;
        if (inputValid)
            op0 = qb::va(context.popVa());
        else
            op0 = "vec4(1e10f)\n";

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
        "$1.x = $1.x*$4;\n";
        glsl = qb::replaceArgs(glsl, {qb::va(op1), op0, qb::tfmr(pos1), scaArg});

        context.pushVa(op1);
        context.pushCode(glsl);

        return;
    }
    context.popTransform();

    std::string op2;
    if (inputValid)
        op2 = qb::va(context.popVa());
    else
        op2 = "vec4(1e10f)\n";
    size_t op3 = context.getNextVa();

    std::string glslPop = "vec4 $1 = opTransformPop($2, $3);\n";
    glslPop = qb::replaceArgs(glslPop, {qb::va(op3), op2, scaArg});

    context.pushVa(op3);
    context.pushCode(glslPop);
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
    "vec4 mkQuat(vec3 axis, float a){\n"
    "    float co = cos(a * 0.5);\n"
    "    float si = sin(a * 0.5);\n"
    "    return vec4(axis * si, co);\n"
    "}\n"
    "vec4 multQuat(vec4 q1, vec4 q2){\n"
    "    return vec4(\n"
    "        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,\n"
    "        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,\n"
    "        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,\n"
    "        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z\n"
    "    );\n"
    "}\n"
    "vec4 mkQuatFromRot(vec3 rots){\n"
    "    vec2 axis = vec2(1.0,0.0);\n"
    "    vec4 qx = mkQuat(axis.xyy, rots.x);\n"
    "    vec4 qy = mkQuat(axis.yxy, rots.y);\n"
    "    vec4 qz = mkQuat(axis.yyx, rots.z);\n"
    "    return multQuat(qx, multQuat(qy,qz));\n"
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
//--------------------------------------------------------------
void Repetition::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& c = attributes->c;
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in1 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = vec4(opRepetition($2.xyz, $3.x, $3.yzw), 0.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in1});
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)RepetitionData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    visitor.sampleInput(result, inputs[0]);
    context.popTransform();
}
//--------------------------------------------------------------
std::string Repetition::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec3 opRepetition(vec3 p, float c, vec3 l){\n"
    "    vec3 q = p-c*clamp(round(p/c),-l,l);\n"
    "    return q;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void Twist::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in;
    if ( inputs[1] && !inputs[1]->isSdf)
        in = visitor.inputOrUniformPlaceholder(result, inputs[1], true) + ".x * 20.0 - 10.0";
    else
        in = qb::in(visitor.getCurrentFrame().uniformPlaceholder()) + ".x";

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = vec4(opTwist($2.xyz, $3), 0.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in});
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)TwistData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    visitor.sampleInput(result, inputs[0]);
    context.popTransform();
}
//--------------------------------------------------------------
std::string Twist::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec3 opTwist(vec3 p, float k){\n"
    "    float c = cos(k*p.y);\n"
    "    float s = sin(k*p.y);\n"
    "    mat2  m = mat2(c,-s,s,c);\n"
    "    vec3  q = vec3(m*p.xz,p.y);\n"
    "    return q.xzy;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void Bend::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in;
    if ( inputs[1] && !inputs[1]->isSdf)
        in = visitor.inputOrUniformPlaceholder(result, inputs[1], true) + ".x * 20.0 - 10.0";
    else
        in = qb::in(visitor.getCurrentFrame().uniformPlaceholder()) + ".x";

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = vec4(opBend($2.xyz, $3), 0.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in});
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)BendData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    visitor.sampleInput(result, inputs[0]);
    context.popTransform();
}
//--------------------------------------------------------------
std::string Bend::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec3 opBend(vec3 p, float k){\n"
    "    float c = cos(k*p.x);\n"
    "    float s = sin(k*p.x);\n"
    "    mat2  m = mat2(c,-s,s,c);\n"
    "    vec3  q = vec3(m*p.xy,p.z);\n"
    "    return q;\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void Elongation::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in;
    if ( inputs[1] && !inputs[1]->isSdf)
        in = visitor.inputOrUniformPlaceholder(result, inputs[1], true) + ".xyz * 4.0 - 2.0";
    else
        in = qb::in(visitor.getCurrentFrame().uniformPlaceholder()) + ".xyz";

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = vec4(opElongate($2.xyz, $3), 0.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in});
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)ElongationData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    bool inputValid = visitor.sampleInput(result, inputs[0]);
    context.popTransform();

    std::string op2;
    if (inputValid)
        op2 = qb::va(context.popVa());
    else
        op2 = "vec4(1e10f)\n";
    size_t op3 = context.getNextVa();

    std::string glslPop = "vec4 $1 = opElongatePop($2, $3.xyz, $4);\n";
    glslPop = qb::replaceArgs(glslPop, {qb::va(op3), op2, qb::tfmr(pos0), in});

    context.pushVa(op3);
    context.pushCode(glslPop);
}
//--------------------------------------------------------------
std::string Elongation::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec3 opElongate(vec3 p, vec3 h){\n"
    "    vec3 q = abs(p)-h;\n"
    "    return max(q,0.0);\n"
    "}\n"
    "vec4 opElongatePop(vec4 v, vec3 p, vec3 h){\n"
    "    vec3 q = abs(p)-h;\n"
    "    return vec4(v.x + min(max(q.x,max(q.y,q.z)),0.0), v.yzw);\n"
    "}\n";
    return std::string(code);
}
//--------------------------------------------------------------
void Symmetry::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    std::string in = qb::in(visitor.getCurrentFrame().uniformPlaceholder());

    size_t pos0 = context.getTransformId();
    size_t pos1 = context.getNextTransform();

    std::string glsl = "vec4 $1 = vec4(opSymmetry($2.xyz, $3.xyz), 0.0);\n";
    glsl = qb::replaceArgs(glsl, {qb::tfmr(pos1), qb::tfmr(pos0), in});
    context.pushCode(glsl);

    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)SymmetryData::TypeId, getOperationCode());

    context.pushTransform(pos1);
    visitor.sampleInput(result, inputs[0]);
    context.popTransform();
}
//--------------------------------------------------------------
std::string Symmetry::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec3 opSymmetry(vec3 p, vec3 s){\n"
    "    p = mix(p, abs(p), s);\n"
    "    return p;\n"
    "}\n";
    return std::string(code);
}
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
void Displacement::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& intensity = attributes->intensity;
    auto& mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t frameId = 0;
    bool inputValid = visitor.sampleInFrame(result, inputs[1], frameId);

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());
    std::string in3 = qb::sa(frameId);

    size_t opId = context.getNextVa();
    size_t tfmrId = context.getTransformId();

    std::string glsl = "vec4 $1 = opDisplacement($2, $3.xyz, $4.xy, $5);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

    if (!inputValid)
    {
        glsl = "vec4 $1 = $2;\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1});
    }

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions("sdfFetcher", qb::getSdfFetcherCode());
    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)DisplacementData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
std::string Displacement::getOperationCode() const
{
    static constexpr std::string_view code =
     "vec4 opDisplacement(vec4 v1, vec3 p, vec2 intensityMode, sampler2D s2d)\n"
     "{\n"
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
void Texturing::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& factor = attributes->factor;
    auto& mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    auto& context = frame.getContext();

    size_t frameId = 0;
    bool inputValid = visitor.sampleInFrame(result, inputs[1], frameId);

    std::string in1 = visitor.inputOrUniformPlaceholder(result, inputs[0], true);
    std::string in2 = qb::in(visitor.getCurrentFrame().uniformPlaceholder());
    std::string in3 = qb::sa(frameId);

    size_t opId = context.getNextVa();
    size_t tfmrId = context.getTransformId();

    std::string glsl = "vec4 $1 = opTexturing($2, $3.xyz, $4.xy, $5);\n";
    glsl = qb::replaceArgs(glsl, {qb::va(opId), in1, qb::tfmr(tfmrId), in2, in3});

    if (!inputValid)
    {
        glsl = "vec4 $1 = $2;\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), in1});
    }

    context.pushVa(opId);
    context.pushCode(glsl);

    frame.setFunctions("sdfFetcher", qb::getSdfFetcherCode());
    visitor.getCurrentFrame().setFunctions((qb::GeometryOperationType)TexturingData::TypeId, getOperationCode());
}
//--------------------------------------------------------------
std::string Texturing::getOperationCode() const
{
    static constexpr std::string_view code =
    "vec4 opTexturing(vec4 v1, vec3 p, vec2 factorMode, sampler2D s2d)\n"
    "{\n"
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

//--------------------------------------------------------------
std::string VoxelOutput::pushSlice(TextureOperationResult& result, TextureOperationInfo* input, float targetZ, float voxelSize, bool uniformRequest)
{
    auto& visitor = result.visitor;
    bool valid = input != nullptr;

    if (valid)
    {
        size_t frameId = visitor.pushFrame(qb::GlslFrame::Type::VoxelPlan);
        auto& currFrame = visitor.getCurrentFrame();
        currFrame.targetZ = targetZ;
        currFrame.voxelSize = voxelSize;
        
        if (uniformRequest)
            input->operation->setUniforms(result, input->attributes, input->inputs);
        else
            input->operation->buildProgram(result, input->attributes, input->inputs);
        visitor.popFrame();
        return qb::sa(frameId);
    }
    
    return "";
}
//--------------------------------------------------------------
void VoxelOutput::buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.type = qb::GlslFrame::Type::Texture;
    auto& context = frame.getContext();

    auto& scanningWork = attributes->scanningWork;
    auto& zPlan = attributes->zPlan;
    auto& voxelSize = attributes->voxelSize;

    float targetZ = zPlan;
    if (scanningWork.initialized && !scanningWork.completed)
        targetZ = scanningWork.getWork<qb::ScanningWork>().zPlan;

    std::string valueId = pushSlice(result, inputs[0], targetZ, voxelSize);

    if(valueId.empty())
    {
        visitor.pushFallback(result);
    }
    else
    {
        size_t opId = context.getNextVa();
        frame.hasUv = true;
        std::string glsl = "vec4 $1 = texture2D($2, uv0).xyzw;\n";
        glsl = qb::replaceArgs(glsl, {qb::va(opId), valueId});
        context.pushVa(opId);
        context.pushCode(glsl);
    }

}