#ifndef QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP
#define QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Union : public SdfOperation
{
    Union();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct SmoothUnion : public SdfOperation
{
    SmoothUnion();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float k = 0.3;
};
//--------------------------------------------------------------
struct HighResSdfOutput : public SdfOperation
{
    HighResSdfOutput();
    ~HighResSdfOutput();
    bool sample(size_t index, qb::RMBuilderVisitor& visitor) override;

    void uiProperties() override;

    int resolution = 10;

    static HighResSdfOutput* defaultOutput;
};

#endif // QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP