#ifndef QUASAR_BELL_IMAGE_OPERATION_HPP
#define QUASAR_BELL_IMAGE_OPERATION_HPP

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <list>

#include "ImageOperation/ImageOperationType.hpp"
#include "ImageOperation/GlslBuilder.hpp"

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
struct ImageOperation;
class JsonValue;

//--------------------------------------------------------------
struct ImageOperationConnection
{
    struct Ref 
    {
        ImageOperation* operation = nullptr;
        size_t index = 0;
    };
    std::vector<Ref> refs;
    ImageOperationDataType type = ImageDataType_Undefined;
    std::string name;
};

struct ImagePreview
{
    size_t glResource = 0;
    size_t glProgram = 0;
    size_t res = 256;
    bool initialized = false;
    bool hasChange = true;
    bool toRecompile = true;

    std::string opCode;

    ~ImagePreview();

    void dirty(bool recompile);
    void initialize(ImageOperation* operation);
    void compute(ImageOperation* operation);
};

//--------------------------------------------------------------
struct ImageOperation
{
    struct Time
    {
        vec2 uv = {0.0f,0.0f};
        vec2 coord = {0.0f,0.0f};
        vec2 size = {256.0f, 256.0f};
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

    void update();
    bool sampleInput(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor);

    void startSamplingGraph();

    virtual std::string name() const;
    virtual void startSampling();
    virtual bool sample(size_t index, const Time& t, qb::GlslBuilderVisitor& visitor);
    
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

    void dirty(bool recompile = false);

    qb::ImageOperationType getNodeType() const;

    ImagePreview preview;

protected:
    void makeInput(const std::string& name, ImageOperationDataType type);
    void makeOutput(const std::string& name, ImageOperationDataType type);
    void makeProperty(const std::string& name, ImageOperationDataType type, void* ptr);
    void makeProperty(const std::string& name, float* ptr, float minVal, float maxVal);
    void makeProperty(const std::string& name, int* ptr, int minVal, int maxVal);
    void uiPreview();
    void uiProperty(int index);
    void uiDebugIo();
    bool _hasCustomData = false;

    virtual std::string getOperationCode() const;
    const qb::ImageOperationType _operationType;

private:
    void updateAllChildren();

    std::vector<ImageOperationConnection> inputs;
    std::vector<ImageOperationConnection> outputs;
    std::vector<PropDesc> propDescs;
};

#endif // QUASAR_BELL_IMAGE_OPERATION_HPP