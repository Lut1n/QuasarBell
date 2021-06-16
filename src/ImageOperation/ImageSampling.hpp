#ifndef QUASAR_BELL_IMAGE_SAMPLING_HPP
#define QUASAR_BELL_IMAGE_SAMPLING_HPP

#include "ImageOperation/ImageOperation.hpp"


struct ImageFilter : public ImageOperation
{
    ImageFilter(qb::ImageOperationType type);
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    virtual void updateKernel();

    std::string getOperationCode() const override;

    Kernel kernel;
    int radius = 1;
};

struct BlurFilter : public ImageFilter
{
    BlurFilter();
    void updateKernel() override;
};


struct SharpenFilter : public ImageFilter
{
    SharpenFilter();
    void updateKernel() override;
};

struct MorphoFilter : public ImageOperation
{
    MorphoFilter();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    int radius = 1;
    int mode = 0;
};

//--------------------------------------------------------------
struct BumpToNormal : public ImageOperation
{
    BumpToNormal();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;
};


#endif // QUASAR_BELL_IMAGE_SAMPLING_HPP