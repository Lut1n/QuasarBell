#ifndef QUASAR_BELL_ENVELOP_OPERATION_HPP
#define QUASAR_BELL_ENVELOP_OPERATION_HPP

#include "signal/operations/SignalOperation.hpp"


//--------------------------------------------------------------
struct EnvelopOperation : public SignalOperation
{
    EnvelopOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float sampleADSR(float t);

    // ADSR
    float attack = 0.1f;
    float decay = 0.2f;
    float sustain = 0.5f;
    float release = 0.8f;
};


#endif // QUASAR_BELL_ENVELOP_OPERATION_HPP