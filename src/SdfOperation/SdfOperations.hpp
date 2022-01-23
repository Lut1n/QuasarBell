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
struct Sphere  : public ImageOperation<SphereData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Box  : public ImageOperation<BoxData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundBox  : public ImageOperation<RoundBoxData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Torus  : public ImageOperation<TorusData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct HexagonalPrism  : public ImageOperation<HexagonalPrismData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct TriangularPrism  : public ImageOperation<TriangularPrismData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Capsule  : public ImageOperation<CapsuleData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Cone  : public ImageOperation<ConeData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundedCone  : public ImageOperation<RoundedConeData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Cylinder  : public ImageOperation<CylinderData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct RoundedCylinder  : public ImageOperation<RoundedCylinderData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Octahedron  : public ImageOperation<OctahedronData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};
//--------------------------------------------------------------
struct Pyramid  : public ImageOperation<PyramidData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Union  : public ImageOperation<UnionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Substraction  : public ImageOperation<SubstractionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Intersection  : public ImageOperation<IntersectionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothUnion  : public ImageOperation<SmoothUnionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothSubstraction  : public ImageOperation<SmoothSubstractionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct SmoothIntersection  : public ImageOperation<SmoothIntersectionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct HighResSdfOutput  : public ImageOperation<HighResSdfOutputData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
};

//--------------------------------------------------------------
struct Transform  : public ImageOperation<TransformData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Repetition  : public ImageOperation<RepetitionData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Twist  : public ImageOperation<TwistData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Bend  : public ImageOperation<BendData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Elongation  : public ImageOperation<ElongationData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Symmetry  : public ImageOperation<SymmetryData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Displacement  : public ImageOperation<DisplacementData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct Texturing  : public ImageOperation<TexturingData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string getOperationCode() const;
};

//--------------------------------------------------------------
struct VoxelOutput : public ImageOperation<VoxelOutputData>
{
    void buildProgramImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;
    void setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs) override;

    std::string pushSlice(TextureOperationResult& result, TextureOperationInfo* input, float targetZ, float voxelSize, bool uniformRequest = false);
};

#endif // QB_SDF_OPERATIONS_HPP
