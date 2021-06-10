#ifndef QUASAR_BELL_IMAGE_SAMPLING_HPP
#define QUASAR_BELL_IMAGE_SAMPLING_HPP

#include "ImageOperation/ImageOperation.hpp"

struct BlurFilter : public ImageOperation
{
    BlurFilter();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    int radius = 1;
};


struct SharpenFilter : public ImageOperation
{
    SharpenFilter();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    int radius = 1;
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