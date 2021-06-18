#ifndef QUASAR_BELL_ENVELOP_OPERATION_HPP
#define QUASAR_BELL_ENVELOP_OPERATION_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct EnvelopOperation : public SignalOperation
{
    EnvelopOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float sampleADSR(float t);

    // ADSR
    float attack = 0.1f;
    float decay = 0.2f;
    float sustain = 0.5f;
    float release = 0.8f;
};


#endif // QUASAR_BELL_ENVELOP_OPERATION_HPP