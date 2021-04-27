#ifndef QUASAR_BELL_WAVEFORM_HPP
#define QUASAR_BELL_WAVEFORM_HPP

#include "signal/operations/SignalOperation.hpp"


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
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

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