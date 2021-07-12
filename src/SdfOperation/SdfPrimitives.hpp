#ifndef QUASAR_BELL_SDF_PRIMITIVES_HPP
#define QUASAR_BELL_SDF_PRIMITIVES_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Sphere : public SdfOperation
{
    Sphere();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r = 0.2f;
    float rgb[3] = {1.0,1.0,1.0};
};

//--------------------------------------------------------------
struct Box : public SdfOperation
{
    Box();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float sx = 0.2f,sy = 0.2f, sz = 0.2f;
    float rgb[3] = {1.0,1.0,1.0};
};

struct Capsule : public SdfOperation
{
    Capsule();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float h = 0.5f, r = 0.2f;
    float rgb[3] = {1.0,1.0,1.0};
};

struct Cone : public SdfOperation
{
    Cone();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    void uiProperties() override;

    float r = 0.2f, h = 0.5f;
    float rgb[3] = {1.0,1.0,1.0};
};

#endif // QUASAR_BELL_SDF_PRIMITIVES_HPP