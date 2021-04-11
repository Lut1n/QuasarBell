#ifndef QUASAR_BELL_SIGNAL_OPERATION_HPP
#define QUASAR_BELL_SIGNAL_OPERATION_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
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
    struct Time
    {
        float t = 0.0f;
        float sec = 0.0f;
        float elapsed = 0.0f;
        float duration = 1.0f;
    };

    SignalOperation() = default;
    virtual ~SignalOperation() = default;

    OperationDataType getInputType(size_t index) const;
    OperationDataType getOuputType(size_t index) const;
    
    void setInput(size_t index, SignalOperation* src, size_t srcIndex);
    void setOuput(size_t index, SignalOperation* dst, size_t dstIndex);

    SignalOperationConnection* getInput(size_t index);
    SignalOperationConnection* getOutput(size_t index);

    SignalOperation* getInputOperation(size_t index);

    void initialize(const std::vector<OperationDataType>& input, const std::vector<OperationDataType>& output);
    void update();
    OperationData sampleInput(size_t index, const Time& t);

    void validateGraph();

    virtual std::string name() const;
    virtual void validate();
    virtual OperationData sample(size_t index, const Time& t);
    
    size_t getInputCount() const;
    size_t getOutputCount() const;

    virtual size_t getPropertyCount() const;
    virtual OperationDataType getPropertyType(size_t i) const;
    virtual std::string getPropertyName(size_t i) const;
    virtual void getProperty(size_t i, std::string& value) const;
    virtual void getProperty(size_t i, float& value) const;
    virtual void getProperty(size_t i, int& value) const;
    virtual void getProperty(size_t i, bool& value) const;
    virtual void setProperty(size_t i, const std::string& value);
    virtual void setProperty(size_t i, float value);
    virtual void setProperty(size_t i, int value);
    virtual void setProperty(size_t i, bool value);

    static void setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx);
    static void remConnection(SignalOperation* op, size_t index);

private:
    
    void updateAllChildren();

    std::vector<SignalOperationConnection> inputs;
    std::vector<SignalOperationConnection> outputs;
};

#endif // QUASAR_BELL_SIGNAL_OPERATION_HPP