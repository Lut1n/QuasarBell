#ifndef QUASAR_BELL_MIX_OPERATION_HPP
#define QUASAR_BELL_MIX_OPERATION_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct MixOperation : public SignalOperation
{
    MixOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
    float delta = 0.0;
};


#endif // QUASAR_BELL_MIX_OPERATION_HPP