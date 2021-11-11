#ifndef QUASAR_BELL_SDF_PRIMITIVES_HPP
#define QUASAR_BELL_SDF_PRIMITIVES_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct SdfPrimitive : public SdfOperation
{
    SdfPrimitive(qb::SdfOperationType opType);
    
    void prepareOperation(qb::GlslBuilderVisitor& visitor, size_t& opId, size_t& tfmrId);
    void setupOperation(qb::GlslBuilderVisitor& visitor, size_t opId, const std::string& glsl);

    float rgb[3] = {1.0,1.0,1.0};
};

//--------------------------------------------------------------
struct Sphere : public SdfPrimitive
{
    Sphere();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r = 0.2f;
};

//--------------------------------------------------------------
struct Box : public SdfPrimitive
{
    Box();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float sx = 0.2f, sy = 0.2f, sz = 0.2f;
};
//--------------------------------------------------------------
struct RoundBox : public SdfPrimitive
{
    RoundBox();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float sx = 0.2f, sy = 0.2f, sz = 0.2f;
    float r = 0.02f;
};
//--------------------------------------------------------------
struct Torus : public SdfPrimitive
{
    Torus();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float t1 = 0.2f, t2 = 0.04f;
};
//--------------------------------------------------------------
struct HexagonalPrism : public SdfPrimitive
{
    HexagonalPrism();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float h1 = 0.2f, h2 = 0.2f;
};
//--------------------------------------------------------------
struct TriangularPrism : public SdfPrimitive
{
    TriangularPrism();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float t1 = 0.2f, t2 = 0.2f;
};
//--------------------------------------------------------------
struct Capsule : public SdfPrimitive
{
    Capsule();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float h = 0.5f, r = 0.2f;
};
//--------------------------------------------------------------
struct Cone : public SdfPrimitive
{
    Cone();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r = 0.2f, h = 0.5f;
};
//--------------------------------------------------------------
struct RoundedCone : public SdfPrimitive
{
    RoundedCone();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r1 = 0.2f, r2 = 0.05f, h = 0.5f;
};
//--------------------------------------------------------------
struct Cylinder : public SdfPrimitive
{
    Cylinder();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r = 0.2f, h = 0.5f;
};
//--------------------------------------------------------------
struct RoundedCylinder : public SdfPrimitive
{
    RoundedCylinder();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r1 = 0.2f, r2 = 0.05f, h = 0.5f;
};
//--------------------------------------------------------------
struct Octahedron : public SdfPrimitive
{
    Octahedron();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float s = 0.2f;
};
//--------------------------------------------------------------
struct Pyramid : public SdfPrimitive
{
    Pyramid();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float h = 0.2f;
};

#endif // QUASAR_BELL_SDF_PRIMITIVES_HPP