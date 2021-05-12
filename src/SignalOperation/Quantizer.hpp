#ifndef QUASAR_BELL_QUANTIZER_HPP
#define QUASAR_BELL_QUANTIZER_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct Quantizer : public SignalOperation
{
    Quantizer();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    int quantity = 0;
};


#endif // QUASAR_BELL_QUANTIZER_HPP