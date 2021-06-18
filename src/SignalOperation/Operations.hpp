#ifndef QUASAR_BELL_OPERATION_HPP
#define QUASAR_BELL_OPERATION_HPP

#include "SignalOperation/SignalOperation.hpp"
#include "Audio/AudioRenderer.hpp"

//--------------------------------------------------------------
struct FloatInput : public SignalOperation
{
    FloatInput();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float value = 0.0;
};
// --------------------------------------------------------------
struct NoiseInput : public SignalOperation
{
    NoiseInput();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;
};
// --------------------------------------------------------------
struct Repeater : public SignalOperation
{
    Repeater();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    int count = 1;
};

//--------------------------------------------------------------
struct CubicSampler : public SignalOperation
{
    CubicSampler();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float value = 0.0;
    float speed = 0.0;
    float acc = 0.0;
    float jerk = 0.0;
    float reset = 0.0;
};

//--------------------------------------------------------------
struct PolynomialSampler : public SignalOperation
{
    PolynomialSampler();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;
    
    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    void uiProperties() override;

    int count = 1;
    std::vector<float> coefs;
    float reset = 0.0;
};

//--------------------------------------------------------------
struct AddOperation : public SignalOperation
{
    AddOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct SubOperation : public SignalOperation
{
    SubOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct MultOperation : public SignalOperation
{
    MultOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct DivOperation : public SignalOperation
{
    DivOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct ClampOperation : public SignalOperation
{
    ClampOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float input1 = 0.0f;
    float minVal = 0.0f;
    float maxVal = 1.0f;
};

//--------------------------------------------------------------
struct AbsOperation : public SignalOperation
{
    AbsOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;
};

//--------------------------------------------------------------
struct TimeScale : public SignalOperation
{
    TimeScale();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float delay = 0.0;
    float scale = 1.0;
};

//--------------------------------------------------------------
struct OutputOperation : public SignalOperation
{
    OutputOperation();
    ~OutputOperation();
    
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    void uiProperties() override;

    void generate(PcmDataBase& pcm);

    float range = 1.0;
    float duration = 1.0;
    int sampleRate = 44100;
    int sampleBits = 16;

    // indexes
    int sampleRateIndex = -1;
    int sampleFormatIndex = -1;
    
    static OutputOperation* defaultOutput;
};


#endif // QUASAR_BELL_OPERATION_HPP