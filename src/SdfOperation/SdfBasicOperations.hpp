#ifndef QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP
#define QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP

#include "SdfOperation/SdfOperation.hpp"

//--------------------------------------------------------------
struct Union : public SdfOperation
{
    Union();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct Substraction : public SdfOperation
{
    Substraction();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct Intersection : public SdfOperation
{
    Intersection();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct SmoothUnion : public SdfOperation
{
    SmoothUnion();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float k = 0.3;
};

//--------------------------------------------------------------
struct SmoothSubstraction : public SdfOperation
{
    SmoothSubstraction();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    float k = 0.3;
};

//--------------------------------------------------------------
struct SmoothIntersection : public SdfOperation
{
    SmoothIntersection();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
    
    float k = 0.3;
};

//--------------------------------------------------------------
struct HighResSdfOutput : public SdfOperation
{
    HighResSdfOutput();
    ~HighResSdfOutput();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    void uiProperties() override;

    int resolution = 10;

    static HighResSdfOutput* defaultOutput;
};

#endif // QUASAR_BELL_SDF_BASIC_OPERATIONS_HPP