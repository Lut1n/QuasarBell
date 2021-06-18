#ifndef QUASAR_BELL_IMAGE_UV_OPERATIONS_HPP
#define QUASAR_BELL_IMAGE_UV_OPERATIONS_HPP

#include "ImageOperation/ImageOperation.hpp"

//--------------------------------------------------------------
struct UvInput : public ImageOperation
{
    UvInput();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;
};

//--------------------------------------------------------------
struct UvDistortion : public ImageOperation
{
    UvDistortion();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    float _force = 0.1f;
};

//--------------------------------------------------------------
struct UvMapping : public ImageOperation
{
    UvMapping();
    bool sample(size_t index, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float uOft = 0.0f;
    float vOft = 0.0f;
    float uFct = 1.0f;
    float vFct = 1.0f;
};


#endif // QUASAR_BELL_IMAGE_UV_OPERATIONS_HPP