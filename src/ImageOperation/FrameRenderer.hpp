#ifndef QB_FRAME_RENDERER_HPP
#define QB_FRAME_RENDERER_HPP

#include "ImageOperation/ImageOperations.hpp"
#include "ImageOperation/GlslProgramPipeline.hpp"

#include <list>
#include <vector>
#include <memory>

struct FramePool
{
    int resolution = 256;
    std::list<size_t> availables;
    std::list<size_t> used;
    
    FramePool(size_t c = 1, int resolution = 256);
    ~FramePool();

    size_t getFrame();
    std::vector<size_t> getFrames(size_t c);

    void freeFrame(size_t gl);
    void freeFrames(const std::vector<size_t> ids);
};

//--------------------------------------------------------------
struct FrameRenderer
{
    std::unordered_map<int, std::unique_ptr<FramePool>> framePools;

    size_t programIndex = 0;
    size_t uniformIndex = 0;
    size_t kernelIndex = 0;
    float resolution = 256.0f;

    void render(size_t frame, qb::GlslProgramPipeline::Descriptor& desc, qb::GlslPipelineData& data, std::list<size_t>& inputTextures);
    void render(TexturePreview* target, qb::GlslProgramPipeline& pipeline, qb::GlslPipelineData& data);
};


#endif // QB_FRAME_RENDERER_HPP
