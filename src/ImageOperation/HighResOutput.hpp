#ifndef QUASAR_BELL_HIGHRES_OUTPUT_HPP
#define QUASAR_BELL_HIGHRES_OUTPUT_HPP

#include "ImageOperation/ImageOperation.hpp"


//--------------------------------------------------------------
struct ColorInput : public ImageOperation
{
    ColorInput();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    void uiProperties() override;

    float r=1.0f,g=1.0f,b=1.0f,a=1.0f;
};

//--------------------------------------------------------------
struct ImageAdd : public ImageOperation
{
    ImageAdd();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;
};

//--------------------------------------------------------------
struct ImageMult : public ImageOperation
{
    ImageMult();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;
};

//--------------------------------------------------------------
struct ImageMix : public ImageOperation
{
    ImageMix();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    float delta;
};

//--------------------------------------------------------------
struct Dynamics : public ImageOperation
{
    Dynamics();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    std::string getOperationCode() const override;

    float brightness = 0.0f;
    float contrast = 1.0f;
};

//--------------------------------------------------------------
struct WhiteNoise : public ImageOperation
{
    WhiteNoise();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    std::string getOperationCode() const override;
};

//--------------------------------------------------------------
struct UvMap : public ImageOperation
{
    UvMap();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;
};

//--------------------------------------------------------------
struct UvDistortion : public ImageOperation
{
    UvDistortion();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    std::string getOperationCode() const override;

    float uOft = 0.0f;
    float vOft = 0.0f;
    float uFct = 1.0f;
    float vFct = 1.0f;
};

struct BlurFilter : public ImageOperation
{
    BlurFilter();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    int radius = 1;
};

//--------------------------------------------------------------
/*struct BumpToNormal : public ImageOperation
{
    BumpToNormal();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    std::string getOperationCode() const override;
};*/

//--------------------------------------------------------------
struct PerlinNoise : public ImageOperation
{
    PerlinNoise();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    std::string getOperationCode() const override;

    int octaves = 4;
    float frequency = 2.0f;
    float persistance = 0.7f;
    float smoothness = 1.0f;
};

//--------------------------------------------------------------
struct HighResOutput : public ImageOperation
{
    HighResOutput();
    bool sample(size_t index, const Time& t, ImageOperationVisitor& data) override;

    int res = 1024;
};


#endif // QUASAR_BELL_HIGHRES_OUTPUT_HPP