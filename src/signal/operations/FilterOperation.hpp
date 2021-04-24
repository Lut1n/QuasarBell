#ifndef QUASAR_BELL_FREQ_FILTER_HPP
#define QUASAR_BELL_FREQ_FILTER_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct FreqFilter : public SignalOperation
{
    FreqFilter();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float offset = 440.0f;
    float length = 200.0f;
    float minGain = -0.7f;
    float maxGain = 0.3f;
    float factor = 1.0f;
};


#endif // QUASAR_BELL_FREQ_FILTER_HPP