#ifndef QB_GEOMETRY_ATTRIBUTES_HPP
#define QB_GEOMETRY_ATTRIBUTES_HPP

#include "App/BaseAttributes.hpp"

#include "GeometryOperation/GeometryOperationType.hpp"
#include "Core/ProgressiveWork.hpp"

//--------------------------------------------------------------
struct PrimitiveData : public BaseAttributes
{
    PrimitiveData(size_t typeId);
    float rgb[3] = {1.0,1.0,1.0};
};
//--------------------------------------------------------------
struct SphereData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Sphere;
    SphereData();

    float r = 0.2f;
};

//--------------------------------------------------------------
struct BoxData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Box;
    BoxData();

    float sx = 0.2f, sy = 0.2f, sz = 0.2f;
};
//--------------------------------------------------------------
struct RoundBoxData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_RoundBox;
    RoundBoxData();

    float sx = 0.2f, sy = 0.2f, sz = 0.2f;
    float r = 0.02f;
};
//--------------------------------------------------------------
struct TorusData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Torus;
    TorusData();

    float t1 = 0.2f, t2 = 0.04f;
};
//--------------------------------------------------------------
struct HexagonalPrismData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_HexPrism;
    HexagonalPrismData();

    float h1 = 0.2f, h2 = 0.2f;
};
//--------------------------------------------------------------
struct TriangularPrismData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_TriPrism;
    TriangularPrismData();

    float t1 = 0.2f, t2 = 0.2f;
};
//--------------------------------------------------------------
struct CapsuleData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Capsule;
    CapsuleData();

    float h = 0.5f, r = 0.2f;
};
//--------------------------------------------------------------
struct ConeData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Cone;
    ConeData();

    float r = 0.2f, h = 0.5f;
};
//--------------------------------------------------------------
struct RoundedConeData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_RoundedCone;
    RoundedConeData();

    float r1 = 0.2f, r2 = 0.05f, h = 0.5f;
};
//--------------------------------------------------------------
struct CylinderData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Cylinder;
    CylinderData();

    float r = 0.2f, h = 0.5f;
};
//--------------------------------------------------------------
struct RoundedCylinderData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_RoundedCylinder;
    RoundedCylinderData();

    float r1 = 0.2f, r2 = 0.05f, h = 0.5f;
};
//--------------------------------------------------------------
struct OctahedronData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Octahedron;
    OctahedronData();

    float s = 0.2f;
};
//--------------------------------------------------------------
struct PyramidData : public PrimitiveData
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Pyramid;
    PyramidData();

    float h = 0.2f;
};

//--------------------------------------------------------------
struct UnionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Union;
    UnionData();
};

//--------------------------------------------------------------
struct SubstractionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Substraction;
    SubstractionData();
};

//--------------------------------------------------------------
struct IntersectionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Intersection;
    IntersectionData();
};

//--------------------------------------------------------------
struct SmoothUnionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_SmoothUnion;
    SmoothUnionData();

    float k = 0.3;
};

//--------------------------------------------------------------
struct SmoothSubstractionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_SmoothSubstraction;
    SmoothSubstractionData();

    float k = 0.3;
};
//--------------------------------------------------------------
struct SmoothIntersectionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_SmoothIntersection;
    SmoothIntersectionData();

    float k = 0.3;
};
//--------------------------------------------------------------
struct HighResSdfOutputData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_HighResOutput;
    HighResSdfOutputData();

    void ui() override;

    int resolution = 8;
    int textureResolution = 256;
};
//--------------------------------------------------------------
struct TransformData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Transform;
    TransformData();

    float x = 0.0f,y = 0.0f, z = 0.0f;
    float rx = 0.0f,ry = 0.0f, rz = 0.0f;
    float s = 1.0f;
};
//--------------------------------------------------------------
struct RepetitionData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Repetition;
    RepetitionData();

    float c = 0.4f;
    int x = 1;
    int y = 1;
    int z = 1;
};
//--------------------------------------------------------------
struct TwistData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Twist;
    TwistData();

    float k = 0.0f;
};
//--------------------------------------------------------------
struct BendData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Bend;
    BendData();

    float k = 0.0f;
};
//--------------------------------------------------------------
struct ElongationData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Elongation;
    ElongationData();

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
//--------------------------------------------------------------
struct SymmetryData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Symmetry;
    SymmetryData();

    int x = 0.0f;
    int y = 0.0f;
    int z = 0.0f;
};
//--------------------------------------------------------------
struct DisplacementData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Displacement;
    DisplacementData();
    
    float intensity = 0.5f;
    int mode = 1;
};
//--------------------------------------------------------------
struct TexturingData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Texturing;
    TexturingData();
    
    float factor = 1.0f;
    int mode = 1;
};
//--------------------------------------------------------------
struct VoxelOutputData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::GeometryOperationType_Voxel;
    VoxelOutputData();

    void ui() override;

    void resetWorks();

    float zPlan = 0.0f;
    float voxelSize = 0.1f;
    bool toExport = false;

    qb::ProgressiveWork scanningWork;
    qb::ProgressiveWork indexingWork;
    qb::ProgressiveWork exportationWork;
    qb::ProgressiveWork optimizationWork;
};

#endif // QB_GEOMETRY_ATTRIBUTES_HPP
