#ifndef QUASAR_BELL_OPERATION_HPP
#define QUASAR_BELL_OPERATION_HPP

#include "SignalOperation/SignalOperation.hpp"
#include "Audio/AudioRenderer.hpp"

//--------------------------------------------------------------
struct FloatInput : public SignalOperation
{
    FloatInput();
    OperationData sample(size_t index, const Time& t) override;

    float value = 0.0;
};
// --------------------------------------------------------------
struct NoiseInput : public SignalOperation
{
    NoiseInput();
    OperationData sample(size_t index, const Time& t) override;
};
// --------------------------------------------------------------
struct Repeater : public SignalOperation
{
    Repeater();
    OperationData sample(size_t index, const Time& t) override;

    int count = 1;
};

//--------------------------------------------------------------
struct CubicSampler : public SignalOperation
{
    CubicSampler();
    OperationData sample(size_t index, const Time& t) override;

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
    OperationData sample(size_t index, const Time& t) override;
    
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
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct SubOperation : public SignalOperation
{
    SubOperation();
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct MultOperation : public SignalOperation
{
    MultOperation();
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct DivOperation : public SignalOperation
{
    DivOperation();
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct ClampOperation : public SignalOperation
{
    ClampOperation();
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float minVal = 0.0f;
    float maxVal = 1.0f;
};

//--------------------------------------------------------------
struct AbsOperation : public SignalOperation
{
    AbsOperation();
    OperationData sample(size_t index, const Time& t) override;
};

//--------------------------------------------------------------
struct TimeScale : public SignalOperation
{
    TimeScale();
    OperationData sample(size_t index, const Time& t) override;

    float delay = 0.0;
    float scale = 1.0;
};

//--------------------------------------------------------------
struct OutputOperation : public SignalOperation
{
    OutputOperation();
    ~OutputOperation();
    
    OperationData sample(size_t index, const Time& t) override;

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