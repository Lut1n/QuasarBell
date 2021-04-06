#ifndef QUASAR_BELL_QUANTIZER_HPP
#define QUASAR_BELL_QUANTIZER_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct Quantizer : public SignalOperation
{
    Quantizer();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    int quantity = 0;
};


#endif // QUASAR_BELL_QUANTIZER_HPP