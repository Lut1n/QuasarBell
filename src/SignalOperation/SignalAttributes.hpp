#ifndef QB_SIGNAL_PROPERTIES_HPP
#define QB_SIGNAL_PROPERTIES_HPP

#include "App/BaseAttributes.hpp"
#include "SignalOperation/OperationType.hpp"
#include "Core/ProgressiveWork.hpp"

//--------------------------------------------------------------
struct EnvelopData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Envelop;

    EnvelopData();

    // ADSR
    float attack = 0.1f;
    float decay = 0.2f;
    float sustain = 0.5f;
    float release = 0.8f;
};
//--------------------------------------------------------------
struct FreqFilterData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Filter;

    FreqFilterData();
    void ui() override;

    float offset = 440.0f;
    float length = 200.0f;
    float minGain = -0.7f;
    float maxGain = 0.3f;
    float factor = 1.0f;
};
//--------------------------------------------------------------
struct HarmonicsData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Harmonics;

    HarmonicsData();
    void ui() override;

    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    int count = 1;
    std::vector<std::pair<float,float>> freqs;
};
//--------------------------------------------------------------
struct KeySamplerData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_KeySampler;

    enum Interpo
    {
        Flat,
        Linear,
        Cubic
    };

    KeySamplerData();

    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    void ui() override;

    int count = 1;
    int interpo = 0;
    std::vector<std::pair<float,float>> keys;
};
//--------------------------------------------------------------
struct MixData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Mix;

    MixData();

    float input1 = 0.0f;
    float input2 = 0.0f;
    float delta = 0.0;
};
//--------------------------------------------------------------
struct FloatData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Float;

    FloatData();

    float value = 0.0;
};
// --------------------------------------------------------------
struct NoiseData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Noise;

    NoiseData();
};
// --------------------------------------------------------------
struct RepeaterData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Repeater;

    RepeaterData();

    int count = 1;
};
//--------------------------------------------------------------
struct CubicSamplerData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_CubicSampler;

    CubicSamplerData();

    float value = 0.0;
    float speed = 0.0;
    float acc = 0.0;
    float jerk = 0.0;
    float reset = 0.0;
};
//--------------------------------------------------------------
struct PolynomialSamplerData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Polynomial;

    PolynomialSamplerData();
    void ui() override;
    
    void saveCustomData(JsonValue& json) override;
    void loadCustomData(JsonValue& json) override;

    int count = 1;
    std::vector<float> coefs;
    float reset = 0.0;
};
//--------------------------------------------------------------
struct AddData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Add;

    AddData();

    float input1 = 0.0f;
    float input2 = 0.0f;
};
//--------------------------------------------------------------
struct SubData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Sub;

    SubData();

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct MultData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Mult;

    MultData();

    float input1 = 0.0f;
    float input2 = 0.0f;
};
//--------------------------------------------------------------
struct DivData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Div;

    DivData();

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct ClampData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Clamp;

    ClampData();

    float input1 = 0.0f;
    float minVal = 0.0f;
    float maxVal = 1.0f;
};
//--------------------------------------------------------------
struct AbsData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Abs;

    AbsData();
};

//--------------------------------------------------------------
struct TimeScaleData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_TimeScale;

    TimeScaleData();

    float delay = 0.0;
    float scale = 1.0;
};
//--------------------------------------------------------------
struct OutputData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_AudioOutput;

    OutputData();
    void ui() override;

    float freq = 440.0f;
    float ampl = 1.0f;
    float duration = 1.0;

    int sampleRate = 44100;
    int sampleBits = 16;

    // indexes
    int sampleRateIndex = -1;
    int sampleFormatIndex = -1;

    std::string path = "./exported/output.wav";
    bool toPlay = false;
    bool toStop = false;
    bool toExport = false;

    qb::ProgressiveWork pcmPlaying;
};
//--------------------------------------------------------------
struct PitchData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Pitch;

    PitchData();
    void ui() override;

    int getMidiIndex() const;
    float getFreq() const;

    int octave = 4;
    int semitone = 9;
};
//--------------------------------------------------------------
struct QuantizerData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Quantizer;

    QuantizerData();

    int quantity = 0;
};
//--------------------------------------------------------------
struct StepData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Step;

    StepData();

    float edge = 0.5f;
    float thenValue = 0.0f;
    float elseValue = 0.0f;
};
//--------------------------------------------------------------
struct WaveformData : public BaseAttributes
{
    static constexpr size_t TypeId = (size_t)qb::OperationType_Waveform;

    enum WaveType
    {
        Sin,
        Saw,
        Square,
        Noise
    };

    WaveformData();
    void ui() override;

    void computeNoise();

    float period = 1.0f;
    float minVal = -1.0f;
    float maxVal = 1.0f;
    int type = 0;
    int noiseSeed = 789456123;
    int noiseSamples = 10;

    std::vector<float> samples;

private:
    int _prev_seed = 0;
    int _prev_samples = 0;
};

#endif // QB_SIGNAL_PROPERTIES_HPP
