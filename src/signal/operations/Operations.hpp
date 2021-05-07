#ifndef QUASAR_BELL_OPERATION_HPP
#define QUASAR_BELL_OPERATION_HPP

#include "signal/operations/SignalOperation.hpp"

//--------------------------------------------------------------
struct FloatInput : public SignalOperation
{
    FloatInput();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float value = 0.0;
};
// --------------------------------------------------------------
struct NoiseInput : public SignalOperation
{
    NoiseInput();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
};
// --------------------------------------------------------------
struct Repeater : public SignalOperation
{
    Repeater();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    int count = 1;
};

//--------------------------------------------------------------
struct CubicSampler : public SignalOperation
{
    CubicSampler();
    void validate() override;
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
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    int count = 1;
    std::vector<float> coefs;
    float reset = 0.0;
};

//--------------------------------------------------------------
struct AddOperation : public SignalOperation
{
    AddOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct SubOperation : public SignalOperation
{
    SubOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct MultOperation : public SignalOperation
{
    MultOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct DivOperation : public SignalOperation
{
    DivOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct ClampOperation : public SignalOperation
{
    ClampOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float minVal = 0.0f;
    float maxVal = 1.0f;
};

//--------------------------------------------------------------
struct AbsOperation : public SignalOperation
{
    AbsOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
};

//--------------------------------------------------------------
struct TimeScale : public SignalOperation
{
    TimeScale();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float delay = 0.0;
    float scale = 1.0;
};

//--------------------------------------------------------------
struct OutputOperation : public SignalOperation
{
    OutputOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float range = 1.0;
    float duration = 1.0;
    int sampleRate = 44100;
    int sampleBits = 16;
};


#endif // QUASAR_BELL_OPERATION_HPP