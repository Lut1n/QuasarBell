#ifndef QUASAR_BELL_STEP_OP_HPP
#define QUASAR_BELL_STEP_OP_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct StepOperation : public SignalOperation
{
    StepOperation();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    float edge = 0.5f;
    float thenValue = 0.0f;
    float elseValue = 0.0f;
};


#endif // QUASAR_BELL_STEP_OP_HPP