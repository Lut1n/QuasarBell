#ifndef QUASAR_BELL_OPERATION_HPP
#define QUASAR_BELL_OPERATION_HPP

#include "signal/operations/SignalOperation.hpp"

//--------------------------------------------------------------
struct FloatInput : public SignalOperation
{
    FloatInput();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;

    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float value = 0.0;
};

//--------------------------------------------------------------
struct LinearInput : public SignalOperation
{
    LinearInput();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float value = 0.0;
    float speed = 0.0;
    float acc = 0.0;
};

//--------------------------------------------------------------
struct AddOperation : public SignalOperation
{
    AddOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct MultOperation : SignalOperation
{
    MultOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
    
    size_t getPropertyCount() const override;
    std::string getPropertyName(size_t i) const override;
    OperationDataType getPropertyType(size_t i) const override;
    void getProperty(size_t i, float& value) const override;
    void setProperty(size_t i, float value) override;

    float input1 = 0.0f;
    float input2 = 0.0f;
};

//--------------------------------------------------------------
struct OutputOperation : SignalOperation
{
    OutputOperation();
    void validate() override;
    OperationData sample(size_t index, const Time& t) override;
};


#endif // QUASAR_BELL_OPERATION_HPP