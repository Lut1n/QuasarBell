#include "ImageOperation/ImageOperations.hpp"

//--------------------------------------------------------------
void ColorInput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.pushInput({attributes->r,attributes->g,attributes->b,attributes->a});
}
//--------------------------------------------------------------
void Construct3f::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto c1 = attributes->c1;
    auto c2 = attributes->c2;
    auto c3 = attributes->c3;

    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {c1,c1,c1,1.0f});
    visitor.inputOrUniform(result, inputs[1], {c2,c2,c2,1.0f});
    visitor.inputOrUniform(result, inputs[2], {c3,c3,c3,1.0f});
}
//--------------------------------------------------------------
void Split3f::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto index = attributes->index;
    auto r = attributes->r;
    auto g = attributes->g;
    auto b = attributes->b;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    frame.pushInput({r,g,b,(float)index});
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void DirectionalSignal::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto directionX = attributes->directionX;
    auto directionY = attributes->directionY;
    auto frequency = attributes->frequency;
    auto amplitude = attributes->amplitude;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.pushInput({directionX,directionY,frequency,amplitude});
}
//--------------------------------------------------------------
void RadialSignal::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto centerX = attributes->centerX;
    auto centerY = attributes->centerY;
    auto frequency = attributes->frequency;
    auto amplitude = attributes->amplitude;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.pushInput({centerX,centerY,frequency,amplitude});
}
//--------------------------------------------------------------
void Dynamics::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto brightness = attributes->brightness;
    auto contrast = attributes->contrast;

    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1.0f,1.0f,1.0f,1.0f});
    visitor.inputOrUniform(result, inputs[1], {brightness,brightness,brightness,brightness});
    visitor.inputOrUniform(result, inputs[2], {contrast,contrast,contrast,contrast});
}
//--------------------------------------------------------------
void HighResOutput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    visitor.inputOrUniform(result, inputs[0], {1.0f,1.0f,1.0f,1.0f});
}
//--------------------------------------------------------------
void TimeInput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    float ti = (float)RenderInterface::getTime();
    frame.pushInput({ti,ti,ti,ti});
}
//--------------------------------------------------------------
void UvInput::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
}
//--------------------------------------------------------------
void SphericalCoord::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
}
//--------------------------------------------------------------
void UvDistortion::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto force = attributes->force;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    
    visitor.sampleInput(result, inputs[0], true);
    visitor.sampleInput(result, inputs[1], true);

    frame.pushInput({force,force,force,force});
}
//--------------------------------------------------------------
void UvMapping::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto uOft = attributes->uOft;
    auto vOft = attributes->vOft;
    auto uFct = attributes->uFct;
    auto vFct = attributes->vFct;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    
    bool inputValid = visitor.sampleInput(result, inputs[1], true); // uv map
    if(!inputValid)
        frame.pushInput({uOft, vOft, uFct, vFct});
    
    visitor.sampleInput(result, inputs[0], true);
}
//--------------------------------------------------------------
void BlurFilter::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    attributes->updateKernel();
    
    auto radius = attributes->radius;
    auto& kernel = attributes->kernel;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(visitor.sampleInFrame(result, inputs[0], frameId, true))
    {
        frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        frame.pushKernel(kernel);
    }
}
//--------------------------------------------------------------
void SharpenFilter::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    attributes->updateKernel();
    
    auto radius = attributes->radius;
    auto& kernel = attributes->kernel;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(visitor.sampleInFrame(result, inputs[0], frameId, true))
    {
        frame.pushInput({(float)radius, 0.0, 0.0, 0.0});
        frame.pushKernel(kernel);
    }
}
//--------------------------------------------------------------
void MorphoFilter::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto radius = attributes->radius;
    auto mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    size_t frameId = 0;
    if(visitor.sampleInFrame(result, inputs[0], frameId, true))
    {
        float radiusf = (float)radius;
        visitor.inputOrUniform(result, inputs[1], {radiusf,radiusf,radiusf,radiusf});

        frame.pushInput({(float)mode, 0.0, 0.0, 0.0});
    }
}
//--------------------------------------------------------------
void BumpToNormal::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    size_t ctx = 0;
    visitor.sampleInContext(result, inputs[0], ctx, true);
}
//--------------------------------------------------------------
void WhiteNoise::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
}
//--------------------------------------------------------------
void ValueNoise::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto smoothness = attributes->smoothness;
    auto octaves = attributes->octaves;

    visitor.inputOrUniform(result, inputs[0], {frequency,frequency,frequency,frequency});
    visitor.inputOrUniform(result, inputs[1], {persistance,persistance,persistance,persistance});
    visitor.inputOrUniform(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    frame.pushInput({(float)octaves, 0.0, 0.0, 0.0});
}
//--------------------------------------------------------------
void GradientNoise::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto smoothness = attributes->smoothness;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    visitor.inputOrUniform(result, inputs[0], {frequency,frequency,frequency,frequency});
    visitor.inputOrUniform(result, inputs[1], {persistance,persistance,persistance,persistance});
    visitor.inputOrUniform(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    frame.pushInput({(float)octaves, 0.0, 0.0, 0.0});
}
//--------------------------------------------------------------
void SimplexNoise::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto smoothness = attributes->smoothness;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    visitor.inputOrUniform(result, inputs[0], {frequency,frequency,frequency,frequency});
    visitor.inputOrUniform(result, inputs[1], {persistance,persistance,persistance,persistance});
    visitor.inputOrUniform(result, inputs[2], {smoothness,smoothness,smoothness,smoothness});

    frame.pushInput({(float)octaves, 0.0, 0.0, 0.0});
}
//--------------------------------------------------------------
void VoronoiNoise::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto frequency = attributes->frequency;
    auto persistance = attributes->persistance;
    auto octaves = attributes->octaves;
    auto mode = attributes->mode;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();

    visitor.inputOrUniform(result, inputs[0], {frequency,frequency,frequency,frequency});
    visitor.inputOrUniform(result, inputs[1], {persistance,persistance,persistance,persistance});

    frame.pushInput({(float)octaves, (float)mode, 0.0, 0.0});
}
//--------------------------------------------------------------
void Mandelbrot::setUniformsImpl(TextureOperationResult& result, AttributeType* attributes, InputInfos& inputs)
{
    auto iterations = attributes->iterations;
    auto oftx = attributes->oftx;
    auto ofty = attributes->ofty;
    auto scale = attributes->scale;

    auto& visitor = result.visitor;
    auto& frame = visitor.getCurrentFrame();
    frame.pushInput({(float)iterations, oftx, ofty, scale});
}