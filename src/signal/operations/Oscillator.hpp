#ifndef QUASAR_BELL_OSCILLATOR_HPP
#define QUASAR_BELL_OSCILLATOR_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct Oscillator : public SignalOperation
{
    Oscillator();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float freq = 440.0f;
    float ampl = 1.0f;

private:
    float phase = 0.0f;
};


#endif // QUASAR_BELL_OSCILLATOR_HPP