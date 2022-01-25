#include "ImageOperation/FrameRenderer.hpp"


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

void FrameRenderer::render(size_t frame, qb::GlslProgramPipeline::Descriptor& desc, qb::GlslPipelineData& data, std::list<size_t>& inputTextures)
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
    constexpr size_t UNALLOCATED = std::numeric_limits<size_t>::max(); // when frame is not instanciated

    auto& framePool = framePools[pipeline.resolution];
    if (!framePool)
        framePool = std::make_unique<FramePool>(1, pipeline.resolution);

    size_t count = pipeline.orderedDescriptors.size();

    programIndex = 0;
    uniformIndex = 0;
    kernelIndex = 0;
    resolution = (float) pipeline.resolution;
    
    std::list<size_t> programStack;
    std::list<size_t> frameStack;
    std::unordered_map<size_t, size_t> inputNeeded;

    auto desc = pipeline.orderedDescriptors[programIndex];

    programStack.push_back(programIndex);
    frameStack.push_back(UNALLOCATED);
    inputNeeded[programIndex] = desc.textureCount;

    bool finalFrame = programIndex == count - 1;

    do
    {
        // find next program to execute
        while(inputNeeded[programIndex] == 0 && !finalFrame)
        {
            programIndex++;
            finalFrame = programIndex == count - 1;
            programStack.push_back(programIndex);
            frameStack.push_back(UNALLOCATED);
            desc = pipeline.orderedDescriptors[programIndex];
            inputNeeded[programIndex] = desc.textureCount;
        }

        // get current program and current frame
        size_t program = programStack.back();
        size_t frame = frameStack.back();
        programStack.pop_back();
        frameStack.pop_back();

        std::list<size_t> inputFrames;
        std::list<size_t> inputPrograms;

        // collect inputs
        for(size_t i=0; i< desc.textureCount; ++i)
        {
            int inputProgram = programStack.back();
            size_t inputFrame = frameStack.back();
            programStack.pop_back();
            frameStack.pop_back();

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
            if (inputFrame == UNALLOCATED)
            {
                // only append when textureCount == 0;
                inputFrame = framePool->getFrame();
                std::list<size_t> ignored;
                render(inputFrame, pipeline.orderedDescriptors[inputProgram], data, ignored);
                *frameIt = inputFrame;
            }
            programIt++;
            frameIt++;
        }

        // render current frame
        if (finalFrame)
            frame = target->glTextureId;
        else
            frame = framePool->getFrame();
        
        render(frame, pipeline.orderedDescriptors[program], data, inputFrames);
        for(auto fid : inputFrames) framePool->freeFrame(fid);
        inputNeeded[program] = 0;

        programStack.push_back(program);
        frameStack.push_back(frame);
    }
    while(!finalFrame);

    target->toRecompile = false;
}