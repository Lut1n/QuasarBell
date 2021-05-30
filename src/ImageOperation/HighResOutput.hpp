#ifndef QUASAR_BELL_HIGHRES_OUTPUT_HPP
#define QUASAR_BELL_HIGHRES_OUTPUT_HPP

#include "ImageOperation/ImageOperation.hpp"


//--------------------------------------------------------------
struct ColorInput : public ImageOperation
{
    ColorInput();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    void uiProperties() override;

    float r=1.0f,g=1.0f,b=1.0f,a=1.0f;
};

//--------------------------------------------------------------
struct ImageMix : public ImageOperation
{
    ImageMix();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    float delta;
};

//--------------------------------------------------------------
struct ImageClamp : public ImageOperation
{
    ImageClamp();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    float edge0 = 0.0f;
    float edge1 = 1.0f;
};

//--------------------------------------------------------------
struct Dynamics : public ImageOperation
{
    Dynamics();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    std::string getOperationCode() const override;

    float brightness = 0.0f;
    float contrast = 1.0f;
};
//--------------------------------------------------------------
struct DirectionalSignal : public ImageOperation
{
    DirectionalSignal();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    float directionX = 1.0f;
    float directionY = 1.0f;
    float frequency = 4.0f;
    float amplitude = 1.0f;
};
//--------------------------------------------------------------
struct RadialSignal : public ImageOperation
{
    RadialSignal();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    float centerX = 1.0f;
    float centerY = 1.0f;
    float frequency = 4.0f;
    float amplitude = 1.0f;
};
//--------------------------------------------------------------
struct HighResOutput : public ImageOperation
{
    HighResOutput();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    int res = 1024;
};


#endif // QUASAR_BELL_HIGHRES_OUTPUT_HPP