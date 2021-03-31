#ifndef QUASAR_BELL_SIGNAL_OPERATION_HPP
#define QUASAR_BELL_SIGNAL_OPERATION_HPP

#include <vector>

//--------------------------------------------------------------
enum OperationDataType
{
    DataType_Undefined,
    DataType_Error,
    DataType_Bool,
    DataType_Int,
    DataType_Float,
    
    DataType_Count
};

//--------------------------------------------------------------
struct OperationData
{
    OperationDataType type = DataType_Undefined;
    size_t count = 0;
    union
    {
        int ivec[4];
        float fvec[4];
        bool bvec[4];
    };
};

//--------------------------------------------------------------
struct SignalOperation;

//--------------------------------------------------------------
struct SignalOperationConnection
{
    SignalOperation* operation = nullptr;
    size_t index = 0;
    size_t count = 0;
    OperationDataType type = DataType_Undefined;
};

//--------------------------------------------------------------
struct SignalOperation
{
    SignalOperation() = default;
    virtual ~SignalOperation() = default;

    OperationDataType getInputType(size_t index) const;
    OperationDataType getOuputType(size_t index) const;
    
    void setInput(size_t index, SignalOperation* src, size_t srcIndex);
    void setOuput(size_t index, SignalOperation* dst, size_t dstIndex);

    SignalOperationConnection* getInput(size_t index);
    SignalOperationConnection* getOutput(size_t index);

    void initialize(const std::vector<OperationDataType>& input, const std::vector<OperationDataType>& output);
    void update();
    OperationData getInputData(size_t index);
    OperationData getOutputData(size_t index) const;

    virtual void updateData();
    virtual OperationData getData(size_t index, float t);

private:
    
    void updateAllChildren();
    static void setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx);
    static void remConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx);

    std::vector<SignalOperationConnection> inputs;
    std::vector<SignalOperationConnection> outputs;
};

#endif // QUASAR_BELL_SIGNAL_OPERATION_HPP