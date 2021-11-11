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
#include "App/BaseOperationNode.hpp"

#include "Core/Math.hpp"
#include "Core/Vec2.h"

//--------------------------------------------------------------
struct ImageOperation;

//--------------------------------------------------------------
struct ImagePreview
{
    struct RenderFrame
    {
        int resolution = 256;
        size_t glResource = 0;
        size_t glProgram = 0;
        size_t firstFrameUniformId = 0;
        std::vector<std::unique_ptr<RenderFrame>> frames;
        std::string opCode;

        RenderFrame(int resolution = 256);
        ~RenderFrame();

        void reset();
        void compute(qb::GlslFrame& frame);
        void updateUniforms(qb::GlslFrame& frame);
        void render();
    };

    int resolution = 256;
    bool initialized = false;
    bool hasChange = true;
    bool toRecompile = true;

    std::unique_ptr<RenderFrame> renderFrame;

    void dirty(bool recompile);
    void compute(ImageOperation* operation);
};

//--------------------------------------------------------------
struct ImageOperation : public BaseOperation
{
    ImageOperation(qb::ImageOperationType opType);
    virtual ~ImageOperation() = default;

    void update();
    bool sampleInput(size_t index, qb::GlslBuilderVisitor& visitor);
    std::string pushOpOrInput(size_t index, qb::GlslBuilderVisitor& visitor, const vec4& uniform);

    virtual std::string name() const;
    bool sample(size_t index, BaseOperationVisitor& visitor) override;
    virtual bool sample(size_t index, qb::GlslBuilderVisitor& visitor);
    
    void uiPreview() override;

    void onInputConnectionChanged() override;
    void onPropertiesChanged() override;
    void dirty(bool recompile = false) override;

    qb::ImageOperationType getNodeType() const;

    ImagePreview preview;

protected:
    virtual std::string getOperationCode() const;
    const qb::ImageOperationType _operationType;

private:
    void updateAllChildren();
};

#endif // QUASAR_BELL_IMAGE_OPERATION_HPP