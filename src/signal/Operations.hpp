#ifndef QUASAR_BELL_OPERATION_HPP
#define QUASAR_BELL_OPERATION_HPP

#include "signal/SignalOperation.hpp"

//--------------------------------------------------------------
struct FloatInput : public SignalOperation
{
    FloatInput();
    void updateData() override;
    OperationData getData(size_t index, float t) override;
//private:
    // OperationData data;
    float value;
};

//--------------------------------------------------------------
struct AddOperation : public SignalOperation
{
    AddOperation();
    void updateData() override;
    OperationData getData(size_t index, float t) override;
//private:
    // OperationData data;
};

//--------------------------------------------------------------
struct MultOperation : SignalOperation
{
    MultOperation();
    void updateData() override;
    OperationData getData(size_t index, float t) override;
//private:
    //OperationData data;
};

//--------------------------------------------------------------
struct Ondulator : SignalOperation
{
    Ondulator();
    void updateData() override;
    OperationData getData(size_t index, float t) override;
//private:
    //OperationData data;
};

//--------------------------------------------------------------
struct OutputOperation : SignalOperation
{
    OutputOperation();
    void updateData() override;
    OperationData getData(size_t index, float t) override;
    
    float sample(float t);
//private:
    //OperationData data;

    float duration;
};


#endif // QUASAR_BELL_OPERATION_HPP