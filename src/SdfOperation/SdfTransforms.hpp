#ifndef QUASAR_BELL_SDF_TRANSFORMS_HPP
#define QUASAR_BELL_SDF_TRANSFORMS_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Transform : public SdfOperation
{
    Transform();
    void startSampling(int d) override;
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    bool showGizmo = false;
    float x = 0.0f,y = 0.0f, z = 0.0f;
    float rx = 0.0f,ry = 0.0f, rz = 0.0f;
    float s = 1.0f;
};

#endif // QUASAR_BELL_SDF_TRANSFORMS_HPP