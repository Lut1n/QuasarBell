#ifndef QUASAR_BELL_OSCILLATOR_HPP
#define QUASAR_BELL_OSCILLATOR_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct Oscillator : public SignalOperation
{
    Oscillator();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float freq = 0.0f;
    float ampl = 1.0f;
    int waveform = 0;

private:
    float phase = 0.0f;
};


#endif // QUASAR_BELL_OSCILLATOR_HPP