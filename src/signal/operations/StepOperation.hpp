#ifndef QUASAR_BELL_STEP_OP_HPP
#define QUASAR_BELL_STEP_OP_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct StepOperation : public SignalOperation
{
    StepOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float edge = 0.5f;
    float thenValue = 0.0f;
    float elseValue = 0.0f;
};


#endif // QUASAR_BELL_STEP_OP_HPP