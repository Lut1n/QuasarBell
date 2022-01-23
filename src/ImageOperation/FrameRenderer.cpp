#include "ImageOperation/FrameRenderer.hpp"




/*FramePoolItem::FramePoolItem(int resolution)
{
    gl = RenderInterface::createTarget(resolution,resolution,false);
    valid = true;
}

FramePoolItem::FramePoolItem(FramePoolItem&& other) noexcept
{
    gl = other.gl;
    valid = other.valid;
    other.valid = false;
}

FramePoolItem::~FramePoolItem()
{
    if (valid)
        RenderInterface::deleteTarget((unsigned)gl);
}

FramePoolItem& FramePoolItem::operator=(FramePoolItem&& other) noexcept
{
    gl = other.gl;
    valid = other.valid;
    other.valid = false;
    return *this;
}*/

FramePool::FramePool(size_t c, int resolution)
    : resolution(resolution)
{
    std::cout << "new FramePool (" << resolution << ")" << std::endl;
    for(size_t i=0; i<c; ++i)
        availables.push_back( RenderInterface::createTarget(resolution, resolution, false) );
}

FramePool::~FramePool()
{
    if (used.size() > 0) std::cout << "FramePool: not all frame are free" << std::endl;

    for(auto id : availables)
        RenderInterface::deleteTarget((unsigned) id);
}

size_t FramePool::getFrame()
{
    if (availables.size() == 0)
    {
        std::cout << "no enough available frames in pull" << std::endl;
        availables.push_back(RenderInterface::createTarget(resolution, resolution, false));
    }

    size_t gl = availables.back();
    used.push_back(gl);
    availables.pop_back();

    return gl;
}

std::vector<size_t> FramePool::getFrames(size_t c)
{
    std::vector<size_t> ret(c);
    for(size_t i=0; i<c; ++i) ret[i] = getFrame();

    /*if (c > 0)
        std::cout << "frame used : " << used.size() << "(+ " << c << " )" << std::endl;*/
    return ret;
}

void FramePool::freeFrame(size_t gl)
{
    auto it = std::find(used.begin(), used.end(), gl);
    availables.push_back(gl);
    used.erase(it);
}

void FramePool::freeFrames(const std::vector<size_t> ids)
{
    for(auto id : ids) freeFrame(id);
}


//--------------------------------------------------------------
void FrameRenderer::render(size_t frameId, bool recompile, qb::GlslFrame* glslFrame)
{
    auto frames = currentPool->getFrames(glslFrame->frames.size());
    size_t index = 0;
    for(auto& glsl : glslFrame->frames)
        render(frames[index++], recompile, &glsl);

    size_t glProgram = programs->at(programIndex++);

    if (recompile)
    {
        std::cout << "recompile... " << std::endl;
        std::string opCode = glslFrame->compile();
        RenderInterface::updateCustomProgram((unsigned)glProgram, opCode, glslFrame->needUv());
    }
    // std::cout << "render" << std::endl;

    if (glslFrame->type == qb::GlslFrame::Type::VoxelPlan)
    {
        RenderInterface::debugCheck("Before update voxel uniforms");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, "u_zEpsilon", glslFrame->voxelSize);
        RenderInterface::setInputCustomProgram((unsigned)glProgram, "u_zTargetPlan", glslFrame->targetZ);
        RenderInterface::debugCheck("After update voxel uniforms");
    }

    size_t inId = 0;
    for(auto& input : glslFrame->inputs)
    {
        RenderInterface::debugCheck("Before update uniforms");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, inId++, input);
        RenderInterface::debugCheck("After update uniforms");
    }

    size_t keId = 0;
    for(auto& kernel : glslFrame->kernels)
    {
        RenderInterface::debugCheck("Before update kernel");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, keId++, kernel);
        RenderInterface::debugCheck("After update kernel");
    }
    
    size_t uniformId = 0;
    size_t textureUnit = 0;
    for (auto& fid : frames)
    {
        RenderInterface::debugCheck("Before update samplers");
        RenderInterface::setInputFrameCustomProgram((unsigned)glProgram, fid, textureUnit++, uniformId++);
        RenderInterface::debugCheck("After update samplers");
    }

    RenderInterface::setTarget((unsigned)frameId);
    RenderInterface::clear(0x000000FF);
    RenderInterface::applyCustomProgram((unsigned)glProgram, vec2(0.0f,0.0f), vec2((float)glslFrame->resolution,(float)glslFrame->resolution));

    currentPool->freeFrames(frames);
}

void FrameRenderer::render(TexturePreview* target, qb::GlslFrame* glslFrame)
{
    auto& framePool = framePools[glslFrame->resolution];
    if (!framePool)
        framePool = std::make_unique<FramePool>(1, glslFrame->resolution);

    if (!target->programSet || target->toRecompile)
    {
        size_t c = glslFrame->totalFrameCount();
        std::cout << "instanciate program (" << c << ")" << std::endl;
        target->programSet = std::make_unique<ProgramSet>(c);
        target->glslCode = glslFrame->compile();
    }

    currentPool = framePool.get();
    programs = target->programSet.get();
    programIndex = 0;

    render(target->glTextureId, target->toRecompile, glslFrame);

    if (target->toRecompile)
        std::cout << "total frame allocated: " << (framePool->used.size() + framePool->availables.size()) << std::endl;
    target->toRecompile = false;
}

void FrameRenderer::render(size_t frame, qb::GlslProgramPipeline::Descriptor& desc, qb::GlslPipelineData& data, std::list<size_t>& inputTextures, size_t& uniformIndex, size_t& kernelIndex, float resolution)
{
    size_t glProgram = desc.programId;

    if (desc.voxelPlan)
    {
        RenderInterface::debugCheck("Before update voxel uniforms");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, "u_zEpsilon", data.voxelSize);
        RenderInterface::setInputCustomProgram((unsigned)glProgram, "u_zTargetPlan", data.targetZ);
        RenderInterface::debugCheck("After update voxel uniforms");
    }

    for(int i=0; i<desc.inputCount; ++i)
    {
        RenderInterface::debugCheck("Before update uniforms");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, i, data.inputs[uniformIndex++]);
        RenderInterface::debugCheck("After update uniforms");
    }

    for(int i=0; i<desc.kernelCount; ++i)
    {
        RenderInterface::debugCheck("Before update kernel");
        RenderInterface::setInputCustomProgram((unsigned)glProgram, i, data.kernels[kernelIndex++]);
        RenderInterface::debugCheck("After update kernel");
    }
    
    size_t uniformId = 0;
    size_t textureUnit = 0;
    for (auto& fid : inputTextures)
    {
        RenderInterface::debugCheck("Before update samplers");
        RenderInterface::setInputFrameCustomProgram((unsigned)glProgram, fid, textureUnit++, uniformId++);
        RenderInterface::debugCheck("After update samplers");
    }

    RenderInterface::setTarget((unsigned)frame);
    RenderInterface::clear(0x000000FF);
    RenderInterface::applyCustomProgram((unsigned)glProgram, vec2(0.0f,0.0f), vec2(resolution,resolution));
}

void FrameRenderer::render(TexturePreview* target, qb::GlslProgramPipeline& pipeline, qb::GlslPipelineData& data)
{
    const size_t MAGIC_NUMBER = 6666; // when frame is not instanciated

    auto& framePool = framePools[pipeline.resolution];
    if (!framePool)
        framePool = std::make_unique<FramePool>(1, pipeline.resolution);

    size_t count = pipeline.orderedDescriptors.size();
    // std::cout << "start render: " << count << " program count" << std::endl;

    if (!target->programSet || target->toRecompile)
    {
        target->programSet = std::make_unique<ProgramSet>(count);
        target->glslCode = pipeline.orderedDescriptors[count-1].glsl;
    }

    int programIndex = 0;
    size_t vec4Index = 0;
    size_t kernelIndex = 0;
    
    std::list<int> programStack;
    std::list<size_t> frameStack;
    // std::list<int> depStack;
    programStack.push_back(programIndex);
    frameStack.push_back(MAGIC_NUMBER);
    // depStack.push_back(pipeline.orderedDescriptors[stack.back()].textureCount);

    bool finalFrame = programIndex == count - 1;

    do
    {
        // find next program to execute
        auto desc = pipeline.orderedDescriptors[programStack.back()];
        while(desc.textureCount == 0 && !finalFrame)
        {
            programIndex++;
            programStack.push_back(programIndex);
            frameStack.push_back(MAGIC_NUMBER);
            desc = pipeline.orderedDescriptors[programStack.back()];
            // depStack.push_back(desc.textureCount);
            finalFrame = programIndex == count - 1;
        }

        // get current program and current frame
        int program = programStack.back();
        size_t frame = frameStack.back();
        // int dep = depStack.back();
        programStack.pop_back();
        frameStack.pop_back();
        // depStack.pop_back();

        std::list<size_t> inputFrames;
        std::list<size_t> inputPrograms;

        // collect inputs
        // std::cout << "collect " << desc.textureCount << " inputs" << std::endl;
        for(size_t i=0; i< desc.textureCount; ++i)
        {
            int inputProgram = programStack.back();
            size_t inputFrame = frameStack.back();
            // size_t inputDep = depStack.back();
            programStack.pop_back();
            frameStack.pop_back();
            // depStack.pop_back();
            
            /*if (inputDep > 0)
                std::cout << "FrameRenderer: error dep != 0" << std::endl;*/

            inputFrames.push_front(inputFrame);
            inputPrograms.push_front(inputProgram);
        }

        // render if necessary
        auto programIt = inputPrograms.begin();
        auto frameIt = inputFrames.begin();
        for(size_t i=0; i<desc.textureCount; ++i)
        {
            int inputProgram = *programIt;
            size_t inputFrame = *frameIt;
            if (inputFrame == MAGIC_NUMBER)
            {
                // std::cout << "new frame as input" << std::endl;
                // only append when children count = 0;
                inputFrame = framePool->getFrame();
                std::list<size_t> ignored;
                // std::cout << "render frame " << inputProgram << std::endl;
                render(inputFrame, pipeline.orderedDescriptors[inputProgram], data, ignored, vec4Index, kernelIndex, (float)pipeline.resolution);
                *frameIt = inputFrame;
            }
            programIt++;
            frameIt++;
        }

        // render current frame
        std::cout << "render a frame " << std::endl;
        if (finalFrame)
        {
            // std::cout << "final frame" << std::endl;
            frame = target->glTextureId;    // final frame
        }
        else
        {
            // std::cout << "new frame as current" << std::endl;
            frame = framePool->getFrame();
        }
        
        // std::cout << "render frame " << program << std::endl;
        render(frame, pipeline.orderedDescriptors[program], data, inputFrames, vec4Index, kernelIndex, (float)pipeline.resolution);
        for(auto fid : inputFrames) framePool->freeFrame(fid);

        programStack.push_back(program);
        frameStack.push_back(frame);
        // depStack.push_back(0);
    }
    while(!finalFrame);

    if (target->toRecompile)
        std::cout << "total frame allocated: " << (framePool->used.size() + framePool->availables.size()) << std::endl;
    target->toRecompile = false;
}