#ifndef QUASAR_BELL_QUANTIZER_HPP
#define QUASAR_BELL_QUANTIZER_HPP

#include "SignalOperation/SignalOperation.hpp"


//--------------------------------------------------------------
struct Quantizer : public SignalOperation
{
    Quantizer();
    bool sample(size_t index, qb::PcmBuilderVisitor& visitor) override;

    int quantity = 0;
};


#endif // QUASAR_BELL_QUANTIZER_HPP