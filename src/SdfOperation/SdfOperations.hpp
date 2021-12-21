#ifndef QB_SDF_OPERATIONS_HPP
#define QB_SDF_OPERATIONS_HPP

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <list>

#include "GeometryOperation/GeometryAttributes.hpp"
#include "ImageOperation/ImageOperations.hpp"
// #include "App/BaseOperationNode.hpp"

//#include "Core/Math.hpp"
//#include "Core/Vec2.h"


//--------------------------------------------------------------
struct Sphere  : public ConcretImageOperation<SphereData>
{
    void sample(TextureOperationResult& result, SphereData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Box  : public ConcretImageOperation<BoxData>
{
    void sample(TextureOperationResult& result, BoxData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundBox  : public ConcretImageOperation<RoundBoxData>
{
    void sample(TextureOperationResult& result, RoundBoxData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Torus  : public ConcretImageOperation<TorusData>
{
    void sample(TextureOperationResult& result, TorusData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct HexagonalPrism  : public ConcretImageOperation<HexagonalPrismData>
{
    void sample(TextureOperationResult& result, HexagonalPrismData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct TriangularPrism  : public ConcretImageOperation<TriangularPrismData>
{
    void sample(TextureOperationResult& result, TriangularPrismData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Capsule  : public ConcretImageOperation<CapsuleData>
{
    void sample(TextureOperationResult& result, CapsuleData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Cone  : public ConcretImageOperation<ConeData>
{
    void sample(TextureOperationResult& result, ConeData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundedCone  : public ConcretImageOperation<RoundedConeData>
{
    void sample(TextureOperationResult& result, RoundedConeData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Cylinder  : public ConcretImageOperation<CylinderData>
{
    void sample(TextureOperationResult& result, CylinderData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundedCylinder  : public ConcretImageOperation<RoundedCylinderData>
{
    void sample(TextureOperationResult& result, RoundedCylinderData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Octahedron  : public ConcretImageOperation<OctahedronData>
{
    void sample(TextureOperationResult& result, OctahedronData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Pyramid  : public ConcretImageOperation<PyramidData>
{
    void sample(TextureOperationResult& result, PyramidData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Union  : public ConcretImageOperation<UnionData>
{
    void sample(TextureOperationResult& result, UnionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Substraction  : public ConcretImageOperation<SubstractionData>
{
    void sample(TextureOperationResult& result, SubstractionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Intersection  : public ConcretImageOperation<IntersectionData>
{
    void sample(TextureOperationResult& result, IntersectionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothUnion  : public ConcretImageOperation<SmoothUnionData>
{
    void sample(TextureOperationResult& result, SmoothUnionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothSubstraction  : public ConcretImageOperation<SmoothSubstractionData>
{
    void sample(TextureOperationResult& result, SmoothSubstractionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothIntersection  : public ConcretImageOperation<SmoothIntersectionData>
{
    void sample(TextureOperationResult& result, SmoothIntersectionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct HighResSdfOutput  : public ConcretImageOperation<HighResSdfOutputData>
{
    void sample(TextureOperationResult& result, HighResSdfOutputData* attributes, InputInfos& inputs) override;
};

//--------------------------------------------------------------
struct Transform  : public ConcretImageOperation<TransformData>
{
    void sample(TextureOperationResult& result, TransformData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Repetition  : public ConcretImageOperation<RepetitionData>
{
    void sample(TextureOperationResult& result, RepetitionData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Twist  : public ConcretImageOperation<TwistData>
{
    void sample(TextureOperationResult& result, TwistData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Bend  : public ConcretImageOperation<BendData>
{
    void sample(TextureOperationResult& result, BendData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Elongation  : public ConcretImageOperation<ElongationData>
{
    void sample(TextureOperationResult& result, ElongationData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Symmetry  : public ConcretImageOperation<SymmetryData>
{
    void sample(TextureOperationResult& result, SymmetryData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Displacement  : public ConcretImageOperation<DisplacementData>
{
    void sample(TextureOperationResult& result, DisplacementData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Texturing  : public ConcretImageOperation<TexturingData>
{
    void sample(TextureOperationResult& result, TexturingData* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct VoxelOutput : public ConcretImageOperation<VoxelOutputData>
{
    void sample(TextureOperationResult& result, VoxelOutputData* attributes, InputInfos& inputs) override;

    std::string pushSlice(TextureOperationResult& result, TextureOperationInfo* input, float targetZ, float voxelSize);
};

#endif // QB_SDF_OPERATIONS_HPP
