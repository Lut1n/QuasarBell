#ifndef QUASAR_BELL_PITCH_HPP
#define QUASAR_BELL_PITCH_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct PitchSelector : public SignalOperation
{
    PitchSelector();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    int getMidiIndex() const;
    float getFreq() const;

    int octave = 4;
    int semitone = 9;
};


#endif // QUASAR_BELL_PITCH_HPP