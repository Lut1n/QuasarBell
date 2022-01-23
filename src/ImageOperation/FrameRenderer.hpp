#ifndef QB_FRAME_RENDERER_HPP
#define QB_FRAME_RENDERER_HPP

#include "ImageOperation/ImageOperations.hpp"
#include <list>
#include <vector>
#include <memory>

//--------------------------------------------------------------
/*struct FramePoolItem
{
    size_t gl = 0;
    bool valid = false;
    FramePoolItem(int resolution);
    FramePoolItem(FramePoolItem&& other) noexcept;
    ~FramePoolItem();

    FramePoolItem& operator=(FramePoolItem&& other) noexcept;
};*/

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
    FramePool* currentPool = nullptr;
    ProgramSet* programs;
    size_t programIndex = 0;

    void render(size_t frameId, bool recompile, qb::GlslFrame* glsl);
    void render(TexturePreview* target, qb::GlslFrame* glslFrame);
};


#endif // QB_FRAME_RENDERER_HPP
