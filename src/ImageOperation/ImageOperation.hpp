#ifndef QUASAR_BELL_IMAGE_OPERATION_HPP
#define QUASAR_BELL_IMAGE_OPERATION_HPP

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <list>

#include "ImageOperation/ImageOperationType.hpp"

#include "Core/Math.hpp"
#include "Core/Vec2.h"

//--------------------------------------------------------------
enum ImageOperationDataType
{
    ImageDataType_Undefined,
    ImageDataType_Error,
    ImageDataType_Bool,
    ImageDataType_Int,
    ImageDataType_Float,
    
    ImageDataType_Count
};

//--------------------------------------------------------------
struct ImageOperationContext
{
    std::vector<std::string> collectedOperations; // uv + op
    std::list<size_t> unusedVars;
    std::vector<size_t> uvIndexes;
    std::list<size_t> uvStack;
    
    size_t getUvId()
    {
        if(uvStack.size() > 0)
            return uvStack.back();
        return 0;
    }

    void pushUv(size_t uvid, const std::string& code)
    {
        collectedOperations.push_back(code);
        uvIndexes.push_back(uvid);
        uvStack.push_back(uvid);
    }

    size_t getNextUvId()
    {
        return uvIndexes.size() + 1;
    }

    void popUv()
    {
        uvStack.pop_back();
    }

    void pushOperation(size_t result_id, const std::string& opcode)
    {
        unusedVars.push_back(result_id);
        collectedOperations.push_back(opcode);
    }

    size_t popResultId()
    {
        size_t id = unusedVars.front(); unusedVars.pop_front();
        return id;
    }

    size_t getNextOperationId()
    {
        return collectedOperations.size();
    }
};

//--------------------------------------------------------------
struct ImageOperationGlobal
{
    std::vector<vec4> collectedUniforms;
    std::unordered_map<qb::ImageOperationType, std::string> functions;
    bool useUV = false;

    size_t pushUniform(const vec4& v4)
    {
        size_t id = collectedUniforms.size();
        collectedUniforms.push_back(v4);
        return id;
    }
};

//--------------------------------------------------------------
struct ImageOperationVisitor
{
    ImageOperationGlobal global;
    ImageOperationContext mainContext;
    std::vector<ImageOperationContext> subContexts;
    std::list<size_t> contextStack;

    ImageOperationContext& getContext()
    {
        if(contextStack.size() > 0)
            return subContexts[contextStack.back()];
        return mainContext;
    }

    size_t pushContext()
    {
        size_t idx = subContexts.size();
        subContexts.push_back(ImageOperationContext());
        contextStack.push_back(idx);
        return idx;
    }

    void popContext()
    {
        contextStack.pop_back();
    }
};

//--------------------------------------------------------------
struct ImageOperation;
class JsonValue;

//--------------------------------------------------------------
struct ImageOperationConnection
{
    ImageOperation* operation = nullptr;
    size_t index = 0;
    size_t count = 0;
    ImageOperationDataType type = ImageDataType_Undefined;
    std::string name;
};

struct ImagePreview
{
    // std::array<float, 32> data;
    // float maxVal = 1.0;
    size_t glResource = 0;
    size_t glProgram = 0;
    size_t res = 512;
    bool initialized = false;
    bool hasChange = true;

    std::string opCode;

    ~ImagePreview();

    void dirty();
    void compute(ImageOperation* operation);
};

//--------------------------------------------------------------
struct ImageOperation
{
    struct Time
    {
        vec2 uv = {0.0f,0.0f};
        vec2 coord = {0.0f,0.0f};
        vec2 size = {512.0f, 512.0f};
        mutable ImageOperation* dstOp = nullptr;
    };

    union MinMaxVal
    {
        float f2[2];
        int i2[2];
    };

    using PropDesc = std::tuple<std::string,ImageOperationDataType,void*,MinMaxVal>;

    ImageOperation(qb::ImageOperationType opType);
    virtual ~ImageOperation() = default;

    ImageOperationDataType getInputType(size_t index) const;
    ImageOperationDataType getOuputType(size_t index) const;
    
    void setInput(size_t index, ImageOperation* src, size_t srcIndex);
    void setOuput(size_t index, ImageOperation* dst, size_t dstIndex);

    ImageOperationConnection* getInput(size_t index);
    ImageOperationConnection* getOutput(size_t index);

    ImageOperation* getInputOperation(size_t index);

    void update();
    bool sampleInput(size_t index, const Time& t, ImageOperationVisitor& data);

    void startSamplingGraph();

    virtual std::string name() const;
    virtual void startSampling();
    virtual bool sample(size_t index, const Time& t, ImageOperationVisitor& data);
    
    size_t getInputCount() const;
    size_t getOutputCount() const;

    size_t getPropertyCount() const;
    ImageOperationDataType getPropertyType(size_t i) const;
    std::string getPropertyName(size_t i) const;

    bool hasCustomData() const;
    
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

    virtual void saveCustomData(JsonValue& json);
    virtual void loadCustomData(JsonValue& json);

    static void setConnection(ImageOperation* src, size_t srcIdx, ImageOperation* dst, size_t dstIdx);
    static void remConnection(ImageOperation* op, size_t index);

    virtual void uiProperties();

    void dirty();

    ImagePreview preview;

protected:
    void makeInput(const std::string& name, ImageOperationDataType type);
    void makeOutput(const std::string& name, ImageOperationDataType type);
    void makeProperty(const std::string& name, ImageOperationDataType type, void* ptr);
    void makeProperty(const std::string& name, float* ptr, float minVal, float maxVal);
    void makeProperty(const std::string& name, int* ptr, int minVal, int maxVal);
    void uiPreview();
    void uiProperty(int index);
    bool _hasCustomData = false;

    void addOperationCode(ImageOperationVisitor& data);
    virtual std::string getOperationCode() const;
    const qb::ImageOperationType _operationType;

private:
    void updateAllChildren();

    std::vector<ImageOperationConnection> inputs;
    std::vector<ImageOperationConnection> outputs;
    std::vector<PropDesc> propDescs;
};

#endif // QUASAR_BELL_IMAGE_OPERATION_HPP