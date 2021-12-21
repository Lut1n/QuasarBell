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
        framePool = std::make_unique<FramePool>(8, glslFrame->resolution);

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
    target->toRecompile = false;
}