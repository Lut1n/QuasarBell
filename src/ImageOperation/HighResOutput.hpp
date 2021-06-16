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
struct Construct3f : public ImageOperation
{
    Construct3f();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    float c1=0.0f,c2=0.0f,c3=0.0f;
};

//--------------------------------------------------------------
struct Split3f : public ImageOperation
{
    Split3f();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;
    
    void uiProperties() override;

    float r=1.0f,g=1.0f,b=1.0f;
    int index = 0;
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
struct TimeInput : public ImageOperation
{
    TimeInput();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;
};
//--------------------------------------------------------------
struct HighResOutput : public ImageOperation
{
    HighResOutput();
    ~HighResOutput();
    bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor) override;

    void uiProperties() override;

    int resolution = 10;

    static HighResOutput* defaultOutput;
};


#endif // QUASAR_BELL_HIGHRES_OUTPUT_HPP