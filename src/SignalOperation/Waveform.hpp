#ifndef QUASAR_BELL_WAVEFORM_HPP
#define QUASAR_BELL_WAVEFORM_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct Waveform : public SignalOperation
{
    enum Type
    {
        Sin,
        Saw,
        Square,
        Noise
    };

    Waveform();
    void startSampling() override;
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    void uiProperties() override;

    float sampleFlip(float t);

    float period = 1.0f;
    float minVal = -1.0f;
    float maxVal = 1.0f;
    int type = 0;
    int noiseSeed = 789456123;
    int noiseSamples = 10;

private:
    int _prev_seed = 0;
    int _prev_samples = 0;
    std::vector<float> samples;

    void computeNoise();
};


#endif // QUASAR_BELL_WAVEFORM_HPP