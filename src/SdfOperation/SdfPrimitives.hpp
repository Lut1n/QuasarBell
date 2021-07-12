#ifndef QUASAR_BELL_SDF_PRIMITIVES_HPP
#define QUASAR_BELL_SDF_PRIMITIVES_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Sphere : public SdfOperation
{
    Sphere();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float r = 0.2f;
};

//--------------------------------------------------------------
struct Box : public SdfOperation
{
    Box();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float sx = 0.2f,sy = 0.2f, sz = 0.2f;
};

#endif // QUASAR_BELL_SDF_PRIMITIVES_HPP