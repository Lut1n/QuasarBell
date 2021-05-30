#ifndef QUASAR_BELL_IMAGE_NOISES_HPP
#define QUASAR_BELL_IMAGE_NOISES_HPP

#include "ImageOperation/ImageOperation.hpp"

//--------------------------------------------------------------
struct WhiteNoise : public ImageOperation
{
    WhiteNoise();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct PerlinNoise : public ImageOperation
{
    PerlinNoise();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};

//--------------------------------------------------------------
struct VoronoiNoise : public ImageOperation
{
    VoronoiNoise();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};

#endif // QUASAR_BELL_IMAGE_NOISES_HPP