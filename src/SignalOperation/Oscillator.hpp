#ifndef QUASAR_BELL_OSCILLATOR_HPP
#define QUASAR_BELL_OSCILLATOR_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct Oscillator : public SignalOperation
{
    Oscillator();
    void startSampling() override;
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float freq = 440.0f;
    float ampl = 1.0f;

private:
    std::unordered_map<SignalOperation*, float> phases;
};


#endif // QUASAR_BELL_OSCILLATOR_HPP