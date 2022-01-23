#include "SdfOperation/SdfOperations.hpp"

#include "Core/Quat.hpp"
#include "SdfOperation/VoxelExporter.hpp"

#include <algorithm>
#include <string>

//--------------------------------------------------------------
void Sphere::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    visitor.getCurrentFrame().pushInput({r, rgb[0], rgb[1], rgb[2]});
}
//--------------------------------------------------------------
void Box::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& sx = attributes->sx;
    auto& sy = attributes->sy;
    auto& sz = attributes->sz;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({sx, sy, sz, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void RoundBox::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& sx = attributes->sx;
    auto& sy = attributes->sy;
    auto& sz = attributes->sz;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({sx, sy, sz, r});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Torus::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& t1 = attributes->t1;
    auto& t2 = attributes->t2;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({t1, t2, 0.0, 0.0});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void HexagonalPrism::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h1 = attributes->h1;
    auto& h2 = attributes->h2;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({h1, h2, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void TriangularPrism::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& t1 = attributes->t1;
    auto& t2 = attributes->t2;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({t1, t2, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Capsule::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({h, r, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Cone::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({r, h, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void RoundedCone::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r1 = attributes->r1;
    auto& r2 = attributes->r2;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({r1, r2, h, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Cylinder::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r = attributes->r;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({r, h, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void RoundedCylinder::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;
    auto& r1 = attributes->r1;
    auto& r2 = attributes->r2;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({r1, r2, h, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Octahedron::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& s = attributes->s;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({s, 0.0f, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Pyramid::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& rgb = attributes->rgb;
    auto& h = attributes->h;

    auto& visitor = result.visitor;
    size_t in1 = visitor.getCurrentFrame().pushInput({h, 0.0f, 0.0f, 0.0f});
    size_t in2 = visitor.getCurrentFrame().pushInput({rgb[0], rgb[1], rgb[2], 0.0f});
}
//--------------------------------------------------------------
void Union::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
}
//--------------------------------------------------------------
void Substraction::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
}
//--------------------------------------------------------------
void Intersection::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
}
//--------------------------------------------------------------
void SmoothUnion::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.getCurrentFrame().pushInput({k,k,k,k});
}
//--------------------------------------------------------------
void SmoothSubstraction::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
    std::string in3 = qb::in(visitor.getCurrentFrame().pushInput({k,k,k,k}));
}
//--------------------------------------------------------------
void SmoothIntersection::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.inputOrUniform(result, inputs[1], {1e10f,1e10f,1e10f,1e10f}, true);
    std::string in3 = qb::in(visitor.getCurrentFrame().pushInput({k,k,k,k}));
}
//--------------------------------------------------------------
void HighResSdfOutput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    visitor.inputOrUniform(result,inputs[0], {1.0f,1.0f,1.0f,1.0f}, true);
}
//--------------------------------------------------------------
void Transform::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
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

    size_t pFrameId = 0;
    size_t rFrameId = 0;
    size_t sFrameId = 0;

    bool sampleP = visitor.sampleInFrame(result, inputs[1], pFrameId, true);
    bool sampleR = visitor.sampleInFrame(result, inputs[2], rFrameId, true);
    bool sampleS = visitor.sampleInFrame(result, inputs[3], sFrameId, true);

    if (!sampleP || !sampleS)
    {
        visitor.getCurrentFrame().pushInput({x,y,z,s});
    }

    if (!sampleR)
    {
        quat qx = quat(vec4(1.0,0.0,0.0,0.0),rx * degToRad);
        quat qy = quat(vec4(0.0,1.0,0.0,0.0),ry * degToRad);
        quat qz = quat(vec4(0.0,0.0,1.0,0.0),rz * degToRad);
        quat q = qx*(qy*qz);
        visitor.getCurrentFrame().pushInput(q);
    }
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Repetition::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& c = attributes->c;
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    visitor.getCurrentFrame().pushInput({c,(float)x,(float)y,(float)z});
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Twist::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    if ( inputs[1] && !inputs[1]->isSdf)
        visitor.inputOrUniform(result, inputs[1], {k / 20.0f + 0.5f,0.0f,0.0f,0.0f}, true);
    else
        visitor.getCurrentFrame().pushInput({k,0.0f,0.0f,0.0f});
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Bend::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& k = attributes->k;

    auto& visitor = result.visitor;
    if ( inputs[1] && !inputs[1]->isSdf)
        visitor.inputOrUniform(result, inputs[1], {k / 20.0f + 0.5f,0.0f,0.0f,0.0f}, true);
    else
        visitor.getCurrentFrame().pushInput({k,0.0f,0.0f,0.0f});
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Elongation::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    if ( inputs[1] && !inputs[1]->isSdf)
        visitor.inputOrUniform(result, inputs[1], {x / 4.0f + 0.5f,y / 4.0f + 0.5f,z / 4.0f + 0.5f,0.0f}, true);
    else
        visitor.getCurrentFrame().pushInput({x,y,z,0.0f});
    bool inputValid = visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Symmetry::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& x = attributes->x;
    auto& y = attributes->y;
    auto& z = attributes->z;

    auto& visitor = result.visitor;
    visitor.getCurrentFrame().pushInput({(float)x,(float)y,(float)z,0.0f});
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void Displacement::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& intensity = attributes->intensity;
    auto& mode = attributes->mode;

    auto& visitor = result.visitor;
    size_t frameId = 0;
    visitor.sampleInFrame(result, inputs[1], frameId, true);
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.getCurrentFrame().pushInput({intensity,(float)mode,0.0f,0.0f});
}
//--------------------------------------------------------------
void Texturing::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& factor = attributes->factor;
    auto& mode = attributes->mode;

    auto& visitor = result.visitor;
    size_t frameId = 0;
    visitor.sampleInFrame(result, inputs[1], frameId, true);
    visitor.inputOrUniform(result, inputs[0], {1e10f,1e10f,1e10f,1e10f}, true);
    visitor.getCurrentFrame().pushInput({factor,(float)mode,0.0f,0.0f});
}
//--------------------------------------------------------------
void VoxelOutput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.type = qb::GlslFrame::Type::Texture;

    auto& scanningWork = attributes->scanningWork;
    auto& zPlan = attributes->zPlan;
    auto& voxelSize = attributes->voxelSize;

    float targetZ = zPlan;
    if (scanningWork.initialized && !scanningWork.completed)
        targetZ = scanningWork.getWork<qb::ScanningWork>().zPlan;

    pushSlice(result, inputs[0], targetZ, voxelSize, true);
}