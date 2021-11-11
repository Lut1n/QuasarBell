#ifndef QUASAR_BELL_SDF_TRANSFORMS_HPP
#define QUASAR_BELL_SDF_TRANSFORMS_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Transform : public SdfOperation
{
    Transform();
    void startSampling(int d) override;
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    bool showGizmo = false;
    float x = 0.0f,y = 0.0f, z = 0.0f;
    float rx = 0.0f,ry = 0.0f, rz = 0.0f;
    float s = 1.0f;
};

//--------------------------------------------------------------
struct Repetition : public SdfOperation
{
    Repetition();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float c = 2.0f;
    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;
};

//--------------------------------------------------------------
struct Twist : public SdfOperation
{
    Twist();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float k = 10.0f;
};

//--------------------------------------------------------------
struct Bend : public SdfOperation
{
    Bend();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float k = 10.0f;
};

//--------------------------------------------------------------
struct Elongation : public SdfOperation
{
    Elongation();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    float x = 0.2f;
    float y = 0.2f;
    float z = 0.2f;
};

//--------------------------------------------------------------
struct Symmetry : public SdfOperation
{
    Symmetry();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    int x = 0.0f;
    int y = 0.0f;
    int z = 0.0f;
};

#endif // QUASAR_BELL_SDF_TRANSFORMS_HPP