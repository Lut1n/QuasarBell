#ifndef GUI_BASE_OPERATION_NODE_H
#define GUI_BASE_OPERATION_NODE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Ui/UiNode.h"

//--------------------------------------------------------------
enum class BaseOperationDataType
{
    Undefined,
    Error,
    Bool,
    Int,
    Float
};

// forward
struct BaseOperation;
class JsonValue;

//--------------------------------------------------------------
struct BaseOperationConnection
{
    struct Ref 
    {
        BaseOperation* operation = nullptr;
        size_t index = 0;
    };
    std::vector<Ref> refs;
    BaseOperationDataType type = BaseOperationDataType::Undefined;
    std::string name;
    UiPin::Type pinType = UiPin::TYPE_BOOL;
};

//--------------------------------------------------------------
struct BaseOperationVisitor
{
    virtual ~BaseOperationVisitor() = default;
};

//--------------------------------------------------------------
struct BaseOperation
{
    union MinMaxVal
    {
        float f2[2];
        int i2[2];
    };

    using PropertyWrapper = std::tuple<std::string,BaseOperationDataType,void*,MinMaxVal>;

    BaseOperation(UiPin::Type pinType);
    virtual ~BaseOperation() = default;

    BaseOperationDataType getInputType(size_t index) const;
    BaseOperationDataType getOuputType(size_t index) const;
    
    void setInput(size_t index, BaseOperation* src, size_t srcIndex);
    void setOuput(size_t index, BaseOperation* dst, size_t dstIndex);
    
    virtual size_t getInputCount() const {return inputs.size();}
    virtual size_t getOutputCount() const {return outputs.size();}
    
    BaseOperationConnection* getInput(size_t index);
    BaseOperationConnection* getOutput(size_t index);

    void startSamplingGraph(int d=0);
    virtual void startSampling(int d=0);

    virtual bool sample(size_t index, BaseOperationVisitor& visitor) = 0;
    void remOutput(size_t index, const BaseOperationConnection::Ref& toErase);

    static void setConnection(BaseOperation* src, size_t srcIdx, BaseOperation* dst, size_t dstIdx);
    static void remConnection(BaseOperation* op, size_t index);
    
    size_t getPropertyCount() const;
    BaseOperationDataType getPropertyType(size_t i) const;
    std::string getPropertyName(size_t i) const;
    
    void getProperty(size_t i, std::string& value) const;
    void getProperty(size_t i, float& value) const;
    void getProperty(size_t i, int& value) const;
    void getPropertyMinMax(size_t i, float& minVal, float& maxVal) const;
    void getPropertyMinMax(size_t i, int& minVal, int& maxVal) const;
    void getProperty(size_t i, bool& value) const;
    void setProperty(size_t i, const std::string& value);
    void setProperty(size_t i, float value);
    void setProperty(size_t i, int value);
    void setProperty(size_t i, bool value);
    
    void makeInput(const std::string& name, BaseOperationDataType type);
    void makeOutput(const std::string& name, BaseOperationDataType type);
    void makeInput(const std::string& name, BaseOperationDataType type, UiPin::Type pinType);
    void makeOutput(const std::string& name, BaseOperationDataType type, UiPin::Type pinType);
    void makeProperty(const std::string& name, BaseOperationDataType type, void* ptr);
    void makeProperty(const std::string& name, float* ptr, float minVal, float maxVal);
    void makeProperty(const std::string& name, int* ptr, int minVal, int maxVal);
    void uiProperty(int index);
    void uiDebugIo();
    
    bool hasCustomData() const;
    virtual void saveCustomData(JsonValue& json);
    virtual void loadCustomData(JsonValue& json);

    UiPin::Type getPinType() const {return _pinType;}
    UiPin::Type getInputPinType(size_t i) const {return inputs[i].pinType;}
    UiPin::Type getOutputPinType(size_t i) const {return outputs[i].pinType;}
    
    virtual void uiProperties();
    virtual void uiPreview() = 0;
    
    virtual void onInputConnectionChanged() = 0;
    virtual void onPropertiesChanged() = 0;
    virtual void dirty(bool recompile = false) = 0;

protected:
    std::vector<BaseOperationConnection> inputs;
    std::vector<BaseOperationConnection> outputs;
    std::vector<PropertyWrapper> propertyWrappers;

    UiPin::Type _pinType;
    bool _hasCustomData = false;
};

//--------------------------------------------------------------
struct BaseOperationNode : public UiNode
{
    BaseOperationNode(const std::string& title, size_t nodetypeId, BaseOperation* op);

    BaseOperation* getOperation();

    void displayProperties() override;
    virtual void updatePreview();

    size_t nodeTypeId() const;
private:
    std::unique_ptr<BaseOperation> _operation;
    size_t _nodetypeId;
};

#endif // GUI_BASE_OPERATION_NODE_H
