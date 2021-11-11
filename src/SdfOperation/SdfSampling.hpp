#ifndef QUASAR_BELL_SDF_SAMPLING_HPP
#define QUASAR_BELL_SDF_SAMPLING_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Displacement : public SdfOperation
{
    Displacement();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    float intensity = 0.5f;
    int cubemapMode = 1;
};

//--------------------------------------------------------------
struct Texturing : public SdfOperation
{
    Texturing();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    float factor = 1.0f;
    int cubemapMode = 1;
};

#endif // QUASAR_BELL_SDF_SAMPLING_HPP