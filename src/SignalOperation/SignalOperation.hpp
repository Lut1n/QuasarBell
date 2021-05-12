#ifndef QUASAR_BELL_SIGNAL_OPERATION_HPP
#define QUASAR_BELL_SIGNAL_OPERATION_HPP

#include <iostream>
#include <vector>
#include <array>
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
class JsonValue;

//--------------------------------------------------------------
struct SignalOperationConnection
{
    SignalOperation* operation = nullptr;
    size_t index = 0;
    size_t count = 0;
    OperationDataType type = DataType_Undefined;
    std::string name;
};

struct SignalOperation;

struct SignalPreview
{
    std::array<float, 32> data;
    float maxVal = 1.0;
    bool hasChange = true;

    void dirty();
    void compute(SignalOperation* operation);
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
        mutable SignalOperation* dstOp = nullptr;
    };

    using PropDesc = std::tuple<std::string,OperationDataType,void*>;

    SignalOperation() = default;
    virtual ~SignalOperation() = default;

    OperationDataType getInputType(size_t index) const;
    OperationDataType getOuputType(size_t index) const;
    
    void setInput(size_t index, SignalOperation* src, size_t srcIndex);
    void setOuput(size_t index, SignalOperation* dst, size_t dstIndex);

    SignalOperationConnection* getInput(size_t index);
    SignalOperationConnection* getOutput(size_t index);

    SignalOperation* getInputOperation(size_t index);

    void update();
    OperationData sampleInput(size_t index, const Time& t);

    void validateGraph();

    virtual std::string name() const;
    virtual void validate();
    virtual OperationData sample(size_t index, const Time& t);
    
    size_t getInputCount() const;
    size_t getOutputCount() const;

    size_t getPropertyCount() const;
    OperationDataType getPropertyType(size_t i) const;
    std::string getPropertyName(size_t i) const;

    bool hasCustomData() const;
    
    void getProperty(size_t i, std::string& value) const;
    void getProperty(size_t i, float& value) const;
    void getProperty(size_t i, int& value) const;
    void getProperty(size_t i, bool& value) const;
    void setProperty(size_t i, const std::string& value);
    void setProperty(size_t i, float value);
    void setProperty(size_t i, int value);
    void setProperty(size_t i, bool value);

    virtual void saveCustomData(JsonValue& json);
    virtual void loadCustomData(JsonValue& json);

    static void setConnection(SignalOperation* src, size_t srcIdx, SignalOperation* dst, size_t dstIdx);
    static void remConnection(SignalOperation* op, size_t index);

    virtual void uiProperties();

    SignalPreview preview;

protected:
    // void initialize(const std::vector<OperationDataType>& input, const std::vector<OperationDataType>& output);
    void makeInput(const std::string& name, OperationDataType type);
    void makeOutput(const std::string& name, OperationDataType type);
    void makeProperty(const PropDesc& property);
    void uiProperty(int index);
    bool _hasCustomData = false;

private:
    
    void updateAllChildren();

    std::vector<SignalOperationConnection> inputs;
    std::vector<SignalOperationConnection> outputs;
    std::vector<PropDesc> propDescs;
};

#endif // QUASAR_BELL_SIGNAL_OPERATION_HPP